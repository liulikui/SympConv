#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <string>
#include "DX12RALDevice.h"
#include "Camera.h"
#include "Sphere.h"
#include "Scene.h"
#include <windowsx.h>
#include "Commandline.h"

// 日志文件
std::ofstream logFile;

// 日志函数
extern void logDebug(const std::string& message)
{
    //std::cout << message << std::endl;
    if (logFile.is_open())
    {
        logFile << message << std::endl;
        //logFile.flush();
    }
}

// 初始化日志文件
void initLogFile()
{
    logFile.open("debug_log.txt", std::ios::out | std::ios::trunc);
    if (logFile.is_open())
    {
        logFile << "[LOG] Debug log started." << std::endl;
    }
    else
    {
        std::cerr << "Failed to open log file!" << std::endl;
    }
}

// 关闭日志文件
void closeLogFile()
{
    if (logFile.is_open())
    {
        logFile.flush();
        logFile << "[LOG] Debug log ended." << std::endl;
        logFile.close();
    }
}

// 为了方便使用，定义一个简化的命名空间别名
namespace dx = DirectX;

// 窗口尺寸常量 - 这里会被覆盖为全屏尺寸或自定义尺寸
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 800;

// 全局变量
HWND hWnd = NULL;              // 窗口句柄
HINSTANCE hInstance = NULL;    // 应用程序实例
bool running = true;           // 运行标志
bool debugOutputEnabled = false; // 调试输出开关，默认关闭
bool f9Pressed = false;        // F9键按下标志，用于检测按键状态变化
bool fullscreenMode = false;   // 全屏模式标志，默认false
int customWindowWidth = 1280;  // 自定义窗口宽度，默认1280
int customWindowHeight = 800;  // 自定义窗口高度，默认800
int frameCount = 0;            // 当前帧数计数器
int maxFrames = -1;            // 最大帧数限制（-1表示不限制）

// 相机对象
Camera* camera = nullptr;

// 鼠标控制参数
float yaw = -90.0f;   // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseCaptured = false;

// 键盘状态数组，用于跟踪按键状态
bool keys[256] = { false }; // 假设是标准ASCII键盘

// 模拟暂停状态标志
bool simulationPaused = false;

// 高精度计时器变量
LARGE_INTEGER frequency;
LARGE_INTEGER lastCounter;
float deltaTime = 0.0f;

// 前向声明
void UpdateCamera(const dx::XMVECTOR& position, const dx::XMVECTOR& target, const dx::XMVECTOR& up);

// 布料、渲染设备和场景对象
IRALDevice* device = nullptr;
Scene* scene = nullptr;
Sphere* sphere = nullptr;

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED && device)
        {
            uint32_t width = LOWORD(lParam);
            uint32_t height = HIWORD(lParam);
            device->Resize(width, height);
            
            // 同时更新相机的尺寸
            if (camera)
            {
                camera->Resize(width, height);
            }
            
            // 更新场景渲染资源的尺寸
            if (scene)
            {
                scene->Resize(width, height);
            }
        }
        break;
    case WM_KEYDOWN:
        // 更新键盘状态数组
        keys[wParam] = true;
        
        // 处理一次性按键事件（如ESC）
        switch (wParam)
        {
        case VK_ESCAPE:
            running = false;
            break;
        case VK_SPACE:
            {
                simulationPaused = !simulationPaused;
                const char* statusMsg = simulationPaused ? "Simulation PAUSED" : "Simulation RESUMED";
                logDebug("[SPACE] " + std::string(statusMsg));
            }
            break;
        case VK_F9:
            if (!f9Pressed)
            {
                // 只在按键状态变化时处理
                debugOutputEnabled = !debugOutputEnabled;
                f9Pressed = true;
                
                // 显示调试状态切换消息
                const char* statusMsg = debugOutputEnabled ? "Debug output ENABLED" : "Debug output DISABLED";
                logDebug("[F9] " + std::string(statusMsg));
            }
            break;
        default:
            break;
        }
        break;
    case WM_KEYUP:
        // 更新键盘状态数组
        keys[wParam] = false;
        
        // 重置F9键按下标志
        if (wParam == VK_F9)
        {
            f9Pressed = false;
        }

        break;
    case WM_RBUTTONDOWN:
        // 鼠标右键按下时开始捕获鼠标
        SetCapture(hWnd);
        ShowCursor(FALSE);
        mouseCaptured = true;
        firstMouse = true;
        // 保存当前鼠标位置
        lastX = GET_X_LPARAM(lParam);
        lastY = GET_Y_LPARAM(lParam);
        break;
    case WM_RBUTTONUP:
        // 鼠标右键释放时释放鼠标捕获
        ReleaseCapture();
        ShowCursor(TRUE);
        mouseCaptured = false;
        break;
    case WM_MOUSEMOVE:
        if (mouseCaptured)
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            if (firstMouse) {
                // 忽略第一次移动，避免初始跳跃
                lastX = xPos;
                lastY = yPos;
                firstMouse = false;
                return 0; // 不执行后续旋转逻辑
            }

            // 计算鼠标移动的相对量
            float xoffset = xPos - lastX;
            float yoffset = lastY - yPos; // 注意这里是相反的，因为y坐标是向下增长的

            // 保存当前位置供下次计算
            lastX = xPos;
            lastY = yPos;

            // 使用非常低的灵敏度以获得更平滑的控制
            float sensitivity = 0.001f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            // 计算相机当前朝向
            dx::XMVECTOR pos = camera->GetPosition();
            dx::XMVECTOR target = camera->GetTarget();
            dx::XMVECTOR up = camera->GetUp();
            dx::XMVECTOR front = dx::XMVector3Normalize(dx::XMVectorSubtract(target, pos));
            dx::XMVECTOR right = dx::XMVector3Normalize(dx::XMVector3Cross(front, up));

            // 根据鼠标移动旋转相机朝向
            // 绕up轴旋转（yaw）
            dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationAxis(up, xoffset);
            front = dx::XMVector3TransformNormal(front, rotationMatrix);
            front = dx::XMVector3Normalize(front);

            // 绕right轴旋转（pitch）
            rotationMatrix = dx::XMMatrixRotationAxis(right, yoffset);
            front = dx::XMVector3TransformNormal(front, rotationMatrix);
            front = dx::XMVector3Normalize(front);

            // 更新相机目标
            target = dx::XMVectorAdd(pos, front);
            camera->SetTarget(target);
        }
        break;
    case WM_MOUSEWHEEL:
        // 处理鼠标滚轮（缩放）
        {
            float zoomFactor = 1.0f / 1.1f; // 默认向前滚轮（推进）时使用缩小因子
            if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) // 向后滚轮（拉远）
                zoomFactor = 1.1f; // 向后滚轮时使用放大因子

            dx::XMVECTOR pos = camera->GetPosition();
            dx::XMVECTOR target = camera->GetTarget();
            dx::XMVECTOR dir = dx::XMVectorSubtract(pos, target);
            dir = dx::XMVectorScale(dir, zoomFactor);
            pos = dx::XMVectorAdd(target, dir);
            camera->SetPosition(pos);
        }
        break;
    case WM_CLOSE:
        running = false;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 注册窗口类
BOOL RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT("SympConv");
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    return RegisterClassEx(&wc);
}

// 创建窗口
BOOL CreateWindowApp(HINSTANCE hInstance)
{
    // 注册窗口类
    if (!RegisterWindowClass(hInstance))
    {
        MessageBox(NULL, TEXT("RegisterWindowClass failed!"), TEXT("Error"), MB_ICONERROR);
        return FALSE;
    }

    DWORD windowStyle;
    DWORD exStyle = 0; // 扩展窗口样式
    int windowWidth, windowHeight;
    int xPos, yPos;

    if (fullscreenMode)
    {
        // 获取系统分辨率
        windowWidth = GetSystemMetrics(SM_CXSCREEN);
        windowHeight = GetSystemMetrics(SM_CYSCREEN);
        
        // 全屏样式，WS_POPUP确保没有边框和标题栏
        windowStyle = WS_POPUP;
        // WS_EX_TOPMOST确保窗口在最顶层，覆盖任务栏
        exStyle = WS_EX_TOPMOST;
        
        // 位置在屏幕最左上角
        xPos = 0;
        yPos = 0;
        
        logDebug("Creating fullscreen window: " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight));
    }
    else
    {
        // 使用自定义窗口大小
        windowWidth = customWindowWidth;
        windowHeight = customWindowHeight;
        windowStyle = WS_OVERLAPPEDWINDOW; // 窗口样式
        exStyle = 0; // 默认扩展样式
        
        // 居中窗口
        xPos = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
        yPos = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
        
        logDebug("Creating window: " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight));
    }

    // 创建窗口，使用CreateWindowEx以支持扩展样式
    hWnd = CreateWindowEx(
        exStyle,                  // 扩展窗口样式
        TEXT("SympConv"),  // 窗口类名称
        TEXT("SympConv"),  // 窗口标题
        windowStyle,                // 窗口样式
        xPos, yPos,                 // 窗口位置
        windowWidth, windowHeight,  // 窗口尺寸
        NULL,                       // 父窗口
        NULL,                       // 菜单
        hInstance,                  // 实例句柄
        NULL                        // 附加数据
    );

    if (!hWnd)
    {
        MessageBox(NULL, TEXT("CreateWindow failed!"), TEXT("Error"), MB_ICONERROR);
        return FALSE;
    }
    
    // 如果是全屏模式，确保窗口能够覆盖整个屏幕，包括系统任务栏
    if (fullscreenMode)
    {
        // 使用SetWindowPos确保窗口能够完全覆盖整个屏幕
        // SWP_NOZORDER: 保持窗口的Z顺序不变
        // SWP_SHOWWINDOW: 显示窗口
        // SWP_NOACTIVATE: 不激活窗口
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, windowWidth, windowHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
        
        // 使用SW_MAXIMIZE进一步确保窗口最大化
        ShowWindow(hWnd, SW_MAXIMIZE);
    }
    else
    {
        // 显示窗口
        ShowWindow(hWnd, SW_SHOW);
    }
    
    UpdateWindow(hWnd);

    return TRUE;
}

// 处理Windows消息
void ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// 初始化DirectX 12渲染设备
BOOL InitializeDevice()
{
    std::cout << "  - Converting window title to wide character..." << std::endl;
    // 转换窗口标题为宽字符
    std::wstring windowName(L"SympConv");

    std::cout << "  - Creating DX12RALDevice object..." << std::endl;
    // 使用实际窗口尺寸参数，而不是硬编码值
    int screenWidth, screenHeight;
    
    if (fullscreenMode)
    {
        // 使用系统分辨率
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);
    }
    else
    {
        // 使用自定义窗口尺寸
        screenWidth = customWindowWidth;
        screenHeight = customWindowHeight;
    }
    
    logDebug("Initializing device with resolution: " + std::to_string(screenWidth) + "x" + std::to_string(screenHeight));
    
    // 创建渲染设备实例，传入正确顺序的参数和窗口尺寸
    device = new DX12RALDevice(screenWidth, screenHeight, windowName, hWnd);
    std::cout << "  - DX12RALDevice object created successfully" << std::endl;
    
    // 创建相机对象，使用实际窗口尺寸
    camera = new Camera(screenWidth, screenHeight);
    std::cout << "  - Camera object created successfully" << std::endl;
    
    // 设置相机初始位置和目标
    dx::XMVECTOR cameraPos = dx::XMVectorSet(0.0f, 10.0f, 15.0f, 1.0f); // 直接在布料正前方
    dx::XMVECTOR cameraTarget = dx::XMVectorSet(0.0f, 5.0f, 0.0f, 1.0f); // 直接指向布料中心
    dx::XMVECTOR cameraUp = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    UpdateCamera(cameraPos, cameraTarget, cameraUp);

    std::cout << "  - Calling device->Initialize()..." << std::endl;
    // 初始化渲染设备
    if (!device->Initialize())
    {
        std::cerr << "  - Failed to initialize DX12RALDevice!" << std::endl;
        MessageBox(NULL, TEXT("Failed to initialize DX12RALDevice!"), TEXT("Error"), MB_ICONERROR);
        return FALSE;
    }
    std::cout << "  - DX12RALDevice initialized successfully" << std::endl;
    
    // 创建场景对象
    std::cout << "  - Creating Scene object..." << std::endl;
    scene = new Scene();
    std::cout << "  - Scene object created successfully" << std::endl;
    
    // 初始化场景，创建根签名
    std::cout << "  - Initializing Scene with device..." << std::endl;
    if (!scene->Initialize(device)) {
        std::cerr << "  - scene->Initialize() failed!" << std::endl;
        MessageBox(NULL, TEXT("Failed to initialize Scene!"), TEXT("Error"), MB_ICONERROR);
        return FALSE;
    }
    std::cout << "  - scene->Initialize() succeeded" << std::endl;

    return TRUE;
}

// 全局渲染计数器，用于控制调试输出频率
int globalRenderFrameCount = 0;

// 更新相机的辅助函数
void UpdateCamera(const dx::XMVECTOR& position, const dx::XMVECTOR& target, const dx::XMVECTOR& up)
{
    if (camera)
    {
        camera->UpdateCamera(position, target, up);
    }
}

// 清理资源
void Cleanup()
{
    // 清理场景对象
    if (scene)
    {
        scene->Clear();
    }
    
    if (sphere)
    {
        delete sphere;
    }
    
    // 清理场景对象
    if (scene)
    {
        delete scene;
        scene = nullptr;
    }

    // 清理渲染设备
    if (device)
    {
        device->Cleanup();
        delete device;
        device = nullptr;
    }
    
    // 清理相机对象
    if (camera)
    {
        delete camera;
        camera = nullptr;
    }
}

// main函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 保存实例句柄
    ::hInstance = hInstance;
    
    // 创建一个单独的控制台窗口，以确保输出可见
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    
    // 初始化日志文件
    initLogFile();
    
    // 无条件输出一些基本信息
    logDebug("[TEST] Console window created");
    
    // 创建Commandline对象解析命令行参数
    Commandline cmdLine(lpCmdLine);
    
    // 检查是否需要显示帮助信息
    if (cmdLine.Find("-help"))
    {
        std::wcout << L"SympConv - 命令行参数帮助" << std::endl;
        std::wcout << L"===================================================" << std::endl;
        std::wcout << L"可用的命令行参数：" << std::endl;
        std::wcout << L"  -help                 显示此帮助信息并退出" << std::endl;
        std::wcout << L"  -debug                启用调试输出模式" << std::endl;
        std::wcout << L"  -maxFrames=xxx        设置最大帧数限制（xxx为数字，-1表示不限制）" << std::endl;
        std::wcout << L"程序控制：" << std::endl;
        std::wcout << L"  F9                    切换调试输出开关" << std::endl;
        std::wcout << L"  ESC                   退出程序" << std::endl;
        std::wcout << L"  W/S/A/D               使用WASD键移动摄像机" << std::endl;
        std::wcout << L"  鼠标右键 + 移动       旋转相机视角" << std::endl;
        std::wcout << L"  鼠标滚轮              缩放相机距离" << std::endl;
        
        // 关闭日志文件
        closeLogFile();
        return 0; // 直接退出程序，不创建其他对象
    }

    // 使用Commandline类解析所有命令行参数
    if (cmdLine.Get("-maxFrames=", maxFrames, maxFrames))
    {
        logDebug("Max frames is set by command line parameters to: " + std::to_string(maxFrames));
    }
    // 解析-fullscreen参数
    if (cmdLine.Find("-fullscreen"))
    {
        fullscreenMode = true;
        logDebug("Fullscreen mode is set by command line parameters to: true");
    }
    
    int tempWinWidth = customWindowWidth;
    if (cmdLine.Get("-winWidth=", tempWinWidth, customWindowWidth))
    {
        // 限制窗口宽度不超过系统分辨率
        int systemWidth = GetSystemMetrics(SM_CXSCREEN);
        customWindowWidth = (tempWinWidth > systemWidth) ? systemWidth : tempWinWidth;
        logDebug("Window width is set by command line parameters to: " + std::to_string(customWindowWidth));
    }
    
    int tempWinHeight = customWindowHeight;
    if (cmdLine.Get("-winHeight=", tempWinHeight, customWindowHeight))
    {
        // 限制窗口高度不超过系统分辨率
        int systemHeight = GetSystemMetrics(SM_CYSCREEN);
        customWindowHeight = (tempWinHeight > systemHeight) ? systemHeight : tempWinHeight;
        logDebug("Window height is set by command line parameters to: " + std::to_string(customWindowHeight));
    }
    
    if (cmdLine.Find("-debug"))
    {
        debugOutputEnabled = true;
        std::cout << "[DEBUG] Debug output enabled via command line parameter: " << cmdLine.GetCommandLineString() << std::endl;
    }
    else
    {
        std::cout << "[INFO] Running in normal mode (debug output disabled)" << std::endl;
    }
    
    // 创建窗口
    std::cout << "Creating window..." << std::endl;
    if (!CreateWindowApp(hInstance))
    {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    std::cout << "Window created successfully" << std::endl;
    
    // 初始化DirectX 12渲染设备
    std::cout << "Initializing device..." << std::endl;
    std::cout << "  - Creating DX12RALDevice instance..." << std::endl;
    if (!InitializeDevice())
    {
        std::cerr << "Failed to initialize device" << std::endl;
        MessageBox(hWnd, L"Failed to initialize DirectX 12 device", L"Error", MB_OK | MB_ICONERROR);
        Cleanup();
        return -1;
    }
    std::cout << "Device initialized successfully" << std::endl;

    std::cout << "Sphere object added to scene successfully" << std::endl;

    // 创建并初始化球体对象
    dx::XMFLOAT3 sphereCenter(0.0f, 5.0f, 0.0f);
    float sphereRadius = 2.0f;

    std::cout << "Creating sphere object..." << std::endl;
    sphere = new Sphere(sphereRadius, 32, 32);

    // 设置球体的材质颜色（红色）
    sphere->SetDiffuseColor(dx::XMFLOAT3(1.0f, 0.1f, 0.1f));
    
    // 设置球体的世界矩阵
    sphere->SetPosition(dx::XMFLOAT3(0.0f, 5.0f, 0.0f));
    sphere->SetScale(dx::XMFLOAT3(1.0f, 1.0f, 1.0f));
    sphere->SetRotation(dx::XMFLOAT3(0.0f, 0.0f, 0.0f));
    
    sphere->Initialize(device);

    // 将球体添加到场景中
    scene->AddPrimitive(sphere);
    std::cout << "Sphere object added to scene successfully" << std::endl;

    // 设置场景光源属性
    scene->SetLightPosition(dx::XMFLOAT3(-10.0f, 30.0f, -10.0f));
    scene->SetLightDiffuseColor(dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // 初始化高精度计时器
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastCounter);
    
    logDebug("Entering main loop");

    // 主循环
    while (running)
    {
        // 处理Windows消息
        ProcessMessages();
        
        // 使用高精度计时器获取当前时间
        LARGE_INTEGER currentCounter;
        QueryPerformanceCounter(&currentCounter);
        
        // 帧率限制：最大240FPS
        const float maxFPS = 240.0f;
        const float targetFrameTime = 1.0f / maxFPS;
        
        deltaTime = (float)(static_cast<double>(currentCounter.QuadPart - lastCounter.QuadPart) / static_cast<double>(frequency.QuadPart));

        // 帧率限制
        if (deltaTime < targetFrameTime)
        {
            // 计算需要等待的毫秒数
            float waitTimeMs = targetFrameTime - deltaTime;
            if (waitTimeMs > 0.0f)
            {
                Sleep(static_cast<DWORD>(waitTimeMs * 1000));

                // 再次获取计时器值，确保deltaTime准确
                QueryPerformanceCounter(&currentCounter);
                deltaTime = static_cast<float>(currentCounter.QuadPart - lastCounter.QuadPart) / static_cast<float>(frequency.QuadPart);
            }
        }

        // 更新lastCounter
        lastCounter = currentCounter;
        
        // 增加帧计数器
        frameCount++;

        // 计算FPS并更新窗口标题
        static float fpsUpdateTimer = 0.0f;
        static int fpsCounter = 0;
        fpsUpdateTimer += deltaTime;
        fpsCounter++;
        
        // 每秒更新一次窗口标题
        if (fpsUpdateTimer >= 1.0f)
        {
            float fps = static_cast<float>(fpsCounter) / fpsUpdateTimer;
            
            // 构造新的窗口标题
            std::wstring originalTitle = L"SympConv";
			
            std::wstring newTitle = originalTitle;

            // 更新窗口标题
            SetWindowTextW(hWnd, newTitle.c_str());
            
            // 重置计时器和计数器
            fpsUpdateTimer = 0.0f;
            fpsCounter = 0;
        }

        // 如果设置了最大帧数限制且已达到，则退出程序
        if (maxFrames > 0 && frameCount >= maxFrames)
        {
            std::cout << "Reached maximum frames (" << maxFrames << "), exiting..." << std::endl;
            running = false;
            continue;
        }

        // 定期输出当前帧数
        if (debugOutputEnabled && frameCount % 30 == 0)
        {
            std::cout << "Current frame: " << frameCount << ", deltaTime: " << deltaTime << std::endl;
        }

#ifdef DEBUG_SOLVER
        logDebug("[DEBUG] BeginFrame" + std::to_string(frameCount));
#endif//DEBUG_SOLVER
        device->BeginFrame();

        // 处理键盘输入
        camera->ProcessKeyboardInput(keys, deltaTime);

        // 只有在模拟未暂停时才更新场景
        if (!simulationPaused)
        {
            // 更新场景
            scene->Update(deltaTime);
        }

        scene->Render(camera->GetViewMatrix(), camera->GetProjectionMatrix());

        device->EndFrame();

#ifdef DEBUG_SOLVER
        logDebug("[DEBUG] EndFrame" + std::to_string(frameCount));
#endif//DEBUG_SOLVER

    }
    
    logDebug("Exiting main loop");
    // 清理资源
    Cleanup();
    logDebug("Resources cleaned up");
    
    // 关闭日志文件
    closeLogFile();
    
    return 0;
}
