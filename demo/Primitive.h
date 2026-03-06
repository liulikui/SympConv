#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <DirectXMath.h>
#include <vector>
#include <cstdint>
#include "TRefCountPtr.h"
#include "IRALDevice.h"

// 为了方便使用，定义一个简化的命名空间别名
namespace dx = DirectX;

// 前向声明
class IRALGraphicsCommandList;
class IRALVertexBuffer;
class IRALIndexBuffer;
class IRALConstBuffer;

struct PrimitiveMesh
{
    TRefCountPtr<IRALVertexBuffer> mVertexBuffer;
    TRefCountPtr<IRALIndexBuffer> mIndexBuffer;
};

class Primitive
{
public:
    // 虚析构函数，确保派生类能够正确析构
    virtual ~Primitive() = default;

    // 纯虚函数，更新对象状态
    virtual void Update(IRALGraphicsCommandList* commandList, float deltaTime) = 0;

    // 获取对象的世界变换矩阵
    const dx::XMMATRIX& GetWorldMatrix() const
    {
        return mWorldMatrix;
    }

    // 设置对象的世界变换矩阵
    void SetWorldMatrix(const dx::XMMATRIX& matrix)
    {
        mWorldMatrix = matrix;
    }

    // 设置对象的位置
    void SetPosition(const dx::XMFLOAT3& position);
    const dx::XMFLOAT3& GetPosition() const
    {
        return mPosition;
    }

    // 设置对象的旋转（欧拉角，弧度）
    void SetRotation(const dx::XMFLOAT3& rotation);
    const dx::XMFLOAT3& GetRotation() const
    {
        return mRotation;
    }

    // 设置对象的缩放
    void SetScale(const dx::XMFLOAT3& scale);
    const dx::XMFLOAT3& GetScale() const
    {
        return mScale;
    }

    // 是否可见
    bool IsVisible() const
    {
        return mVisible;
    }

    // 设置可见性
    void SetVisible(bool isVisible)
    {
        mVisible = isVisible;
    }

    // Setup Mesh
    virtual void OnSetupMesh(IRALDevice* device, PrimitiveMesh& mesh)
    {

    }

    // Update Mesh
    virtual void OnUpdateMesh(IRALDevice* device, PrimitiveMesh& mesh)
    {

    }

    void SetDiffuseColor(const dx::XMFLOAT3& color)
    {
        mDiffuseColor = color;
	}

    const dx::XMFLOAT3& GetDiffuseColor() const
    {
        return mDiffuseColor;
	}

    void SetSpecularColor(const dx::XMFLOAT3& color)
    {
        mSpecularColor = color;
	}

    const dx::XMFLOAT3& GetSpecularColor() const
    {
        return mSpecularColor;
	}

    void SetShininess(float value)
    {
        mShininess = value;
	}

    float GetShininess() const
    {
        return mShininess;
	}

protected:
    // 世界变换矩阵
    dx::XMMATRIX mWorldMatrix = dx::XMMatrixIdentity();

    // 对象的位置、旋转和缩放
    dx::XMFLOAT3 mPosition = {0.0f, 0.0f, 0.0f};
    dx::XMFLOAT3 mRotation = {0.0f, 0.0f, 0.0f};
    dx::XMFLOAT3 mScale = {1.0f, 1.0f, 1.0f};

    // 材质颜色
    dx::XMFLOAT3 mDiffuseColor = {1.0f, 1.0f, 1.0f};
    dx::XMFLOAT3 mSpecularColor = {0.5f, 0.5f, 0.5f};
    float mShininess = 16.0f;

    // 是否可见
    bool mVisible = true;

    // 更新世界矩阵
    void UpdateWorldMatrix();
};

#endif // PRIMITIVE_H