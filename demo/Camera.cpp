#include "Camera.h"

Camera::Camera(uint32_t width, uint32_t height)
    : mWidth(width), mHeight(height),
      mFieldOfView(dx::XMConvertToRadians(45.0f)),
      mNearClipPlane(0.1f),
      mFarClipPlane(1000.0f)
{

    mCameraPosition = dx::XMVectorSet(10.0f, 10.0f, 10.0f, 1.0f);
    mCameraTarget = dx::XMVectorZero();
    mCameraUp = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

Camera::~Camera()
{
    // No special cleanup needed
}

void Camera::UpdateCamera(const dx::XMVECTOR& position, const dx::XMVECTOR& target, const dx::XMVECTOR& up)
{
    mCameraPosition = position;
    mCameraTarget = target;
    mCameraUp = up;
    UpdateViewMatrix();
}

void Camera::Resize(uint32_t width, uint32_t height)
{
    mWidth = width;
    mHeight = height;
    UpdateProjectionMatrix();
}

void Camera::SetPosition(const dx::XMVECTOR& position)
{
    mCameraPosition = position;
    UpdateViewMatrix();
}

void Camera::SetTarget(const dx::XMVECTOR& target)
{
    mCameraTarget = target;
    UpdateViewMatrix();
}

void Camera::SetUp(const dx::XMVECTOR& up)
{
    mCameraUp = up;
    UpdateViewMatrix();
}

void Camera::SetFieldOfView(float fov)
{
    mFieldOfView = fov;
    UpdateProjectionMatrix();
}

void Camera::SetNearClipPlane(float nearPlane)
{
    mNearClipPlane = nearPlane;
    UpdateProjectionMatrix();
}

void Camera::SetFarClipPlane(float farPlane)
{
    mFarClipPlane = farPlane;
    UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
    mViewMatrix = dx::XMMatrixLookAtLH(mCameraPosition, mCameraTarget, mCameraUp);
}

void Camera::UpdateProjectionMatrix()
{
    mProjectionMatrix = dx::XMMatrixPerspectiveFovLH(
        mFieldOfView,
        static_cast<float>(mWidth) / static_cast<float>(mHeight),
        mNearClipPlane,
        mFarClipPlane
    );
}

const dx::XMMATRIX& Camera::GetViewMatrix() const
{
    return mViewMatrix;
}

const dx::XMMATRIX& Camera::GetProjectionMatrix() const
{
    return mProjectionMatrix;
}

const dx::XMVECTOR& Camera::GetPosition() const
{
    return mCameraPosition;
}

const dx::XMVECTOR& Camera::GetTarget() const
{
    return mCameraTarget;
}

const dx::XMVECTOR& Camera::GetUp() const
{
    return mCameraUp;
}

// 实现键盘输入处理方法
void Camera::ProcessKeyboardInput(const bool keys[], float deltaTime)
{
    // 计算相机方向向量
    dx::XMVECTOR front = dx::XMVector3Normalize(dx::XMVectorSubtract(mCameraTarget, mCameraPosition));
    dx::XMVECTOR up = mCameraUp;
    dx::XMVECTOR right = dx::XMVector3Normalize(dx::XMVector3Cross(front, up));
    
    // 移动速度
    float moveSpeed = 2.5f * deltaTime;
    
    // 向前移动 (W键)
    if (keys['W'])
    {
        mCameraPosition = dx::XMVectorAdd(mCameraPosition, dx::XMVectorScale(front, moveSpeed));
        mCameraTarget = dx::XMVectorAdd(mCameraTarget, dx::XMVectorScale(front, moveSpeed));
    }
    
    // 向后移动 (S键)
    if (keys['S'])
    {
        mCameraPosition = dx::XMVectorSubtract(mCameraPosition, dx::XMVectorScale(front, moveSpeed));
        mCameraTarget = dx::XMVectorSubtract(mCameraTarget, dx::XMVectorScale(front, moveSpeed));
    }
    
    // 向左移动 (A键)
    if (keys['A'])
    {
        mCameraPosition = dx::XMVectorAdd(mCameraPosition, dx::XMVectorScale(right, moveSpeed));
        mCameraTarget = dx::XMVectorAdd(mCameraTarget, dx::XMVectorScale(right, moveSpeed));
    }
    
    // 向右移动 (D键)
    if (keys['D'])
    {
        mCameraPosition = dx::XMVectorSubtract(mCameraPosition, dx::XMVectorScale(right, moveSpeed));
        mCameraTarget = dx::XMVectorSubtract(mCameraTarget, dx::XMVectorScale(right, moveSpeed));
    }
    
    // 更新视图矩阵
    UpdateViewMatrix();
}
