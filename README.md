# XPBD物理模拟器

这是一个基于XPBD (Extended Position-Based Dynamics) 的实时物理模拟器，使用DirectX 12作为渲染API

## 项目结构

```
SympConv/
├── .gitignore           # Git忽略文件配置
├── CMakeLists.txt       # CMake构建脚本
├── CodingStyle.md       # 编码规范文档
├── README.md            # 项目说明文档
├── LICENSE              # MIT许可证
├── build/               # 构建输出目录
├── demo/                # 示例代码目录
│   ├── Main.cpp         # 主程序入口
│   ├── Scene.cpp        # 场景实现
│   ├── Scene.h          # 场景头文件
│   ├── Camera.cpp       # 相机实现
│   ├── Camera.h         # 相机头文件
│   ├── DX12RALDevice.cpp # DirectX 12设备实现
│   ├── DX12RALDevice.h  # DirectX 12设备头文件
│   ├── DX12RALCommandList.cpp # DirectX 12命令列表实现
│   ├── DX12RALCommandList.h # DirectX 12命令列表头文件
│   ├── DX12RALResource.cpp # DirectX 12资源实现
│   ├── DX12RALResource.h # DirectX 12资源头文件
│   ├── IRALDevice.h     # 渲染抽象层设备接口
│   ├── Mesh.cpp         # 网格实现
│   ├── Mesh.h           # 网格头文件
│   ├── Primitive.cpp    # 图元实现
│   ├── Primitive.h      # 图元头文件
│   ├── Sphere.cpp       # 球体实现
│   ├── Sphere.h         # 球体头文件
│   ├── RALCommandList.h # 渲染命令列表接口
│   ├── RALDataFormat.h  # 数据格式定义
│   ├── RALResource.h    # 渲染资源接口
│   ├── TRefCountPtr.h   # 智能指针实现
│   └── Commandline.h    # 命令行解析
├── include/             # 库头文件目录
│   └── SympConv/        # SympConv命名空间头文件
│       ├── AutoMem.h        # 自动内存管理
│       ├── CollisionDetection.h # 碰撞检测
│       ├── Constraint.h     # 约束基类定义
│       ├── DihedralBendingConstraint.h # 二面角弯曲约束
│       ├── DistanceConstraint.h # 距离约束
│       ├── Export.h         # 导出宏定义
│       ├── LRAConstraint.h  # 低秩模态约束
│       ├── MathUtils.h      # 数学工具
│       ├── Particle.h       # 粒子类定义
│       ├── PhysicScene.h    # 物理场景接口
│       ├── PhysicWorld.h    # 物理世界定义
│       └── SphereCollisionConstraint.h # 球体碰撞约束
└── src/                 # 库源代码目录
    └── PhysicWorld.cpp  # 物理世界实现
```

## 依赖项

项目使用以下库和工具：
- DirectX 12 SDK
- DirectXMath (数学库)
- Windows SDK
- CMake (3.10或更高版本，构建系统)
- Visual Studio 2019或更高版本

## 构建和运行

### Windows (使用Visual Studio)

1. 确保您已安装CMake (3.10或更高版本) 和Visual Studio (推荐2019或更高版本)。
2. 打开命令提示符或PowerShell，导航到项目目录。
3. 创建并进入构建目录：
   ```
   mkdir build
   cd build
   ```
4. 运行CMake配置：
   ```
   cmake ..
   ```
5. 打开生成的解决方案文件，在Visual Studio中选择"Release"配置，并构建解决方案。
6. 运行生成的可执行文件。

## 使用说明

- **相机控制**：
  - W/S/A/D键：移动摄像机位置
  - 鼠标右键 + 移动：旋转相机视角
  - 鼠标滚轮：缩放相机距离
- **程序控制**：
  - F9键：切换调试输出开关
  - ESC键：退出程序
  - 空格键：暂停/继续布料模拟
- **窗口信息**：
    - 窗口尺寸：默认1280×800像素，可通过命令行参数自定义或切换至全屏模式
    - 窗口标题会显示当前帧率、迭代次数、子迭代次数、布料分辨率、LRA约束状态、LRAMaxStretch值和粒子质量（格式："ClothSimulator [Solver::XPBD, FPS:X, Iter:Y, SubIter:Z, Res:WxH, LRA:ON/OFF, MaxStretch:Z, Mass:M]"），其中W和H分别表示布料的宽度和高度分辨率，LRA:ON表示启用LRA约束，LRA:OFF表示禁用LRA约束，Z表示LRA约束的最大拉伸量，M表示每个粒子的质量

## 命令行参数

程序支持以下命令行参数，按功能意义分组如下：

### 基础信息和控制
| 参数 | 描述 | 默认值 |
|------|------|--------|
| `-help` | 显示帮助信息 | 无 |
| `-debug` | 启用调试模式，输出详细日志信息 | 禁用 |
| `-maxFrames=X` | 设置最大帧数限制，达到后程序自动退出 | 无限制 |


### 窗口设置
| 参数 | 描述 | 默认值 |
|------|------|--------|
| `-fullscreen` | 以全屏模式启动程序 | 禁用 |
| `-winWidth=X` | 设置窗口宽度，X为数字，不能超过系统分辨率 | 1280 |
| `-winHeight=X` | 设置窗口高度，X为数字，不能超过系统分辨率 | 800 |

## 许可证

[MIT License](LICENSE)