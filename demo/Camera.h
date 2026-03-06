#pragma once
#include <cstdint>
#include <DirectXMath.h>

namespace dx = DirectX;

class Camera {
public:
    Camera(uint32_t width, uint32_t height);
    ~Camera();

    void UpdateCamera(const dx::XMVECTOR& position, const dx::XMVECTOR& target, const dx::XMVECTOR& up);
    void Resize(uint32_t width, uint32_t height);
    const dx::XMMATRIX& GetViewMatrix() const;
    const dx::XMMATRIX& GetProjectionMatrix() const;
    const dx::XMVECTOR& GetPosition() const;
    const dx::XMVECTOR& GetTarget() const;
    const dx::XMVECTOR& GetUp() const;
    void SetPosition(const dx::XMVECTOR& position);
    void SetTarget(const dx::XMVECTOR& target);
    void SetUp(const dx::XMVECTOR& up);
    void SetFieldOfView(float fov);
    void SetNearClipPlane(float nearPlane);
    void SetFarClipPlane(float farPlane);
    
    // 处理键盘输入的方法
    void ProcessKeyboardInput(const bool keys[], float deltaTime);

private:
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();

    dx::XMVECTOR mCameraPosition;
    dx::XMVECTOR mCameraTarget;
    dx::XMVECTOR mCameraUp;
    dx::XMMATRIX mViewMatrix;
    dx::XMMATRIX mProjectionMatrix;

    uint32_t mWidth;
    uint32_t mHeight;
    float mFieldOfView;
    float mNearClipPlane;
    float mFarClipPlane;
};
