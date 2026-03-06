#include "Primitive.h"
#include <DirectXMath.h>

// 为了方便使用，定义一个简化的命名空间别名
namespace dx = DirectX;

void Primitive::SetPosition(const dx::XMFLOAT3& position)
{
    mPosition = position;
    UpdateWorldMatrix();
}

void Primitive::SetRotation(const dx::XMFLOAT3& rotation)
{
    mRotation = rotation;
    UpdateWorldMatrix();
}

void Primitive::SetScale(const dx::XMFLOAT3& scale)
{
    mScale = scale;
    UpdateWorldMatrix();
}

void Primitive::UpdateWorldMatrix()
{
    // 计算旋转矩阵（基于欧拉角）
    dx::XMMATRIX rotX = dx::XMMatrixRotationX(mRotation.x);
    dx::XMMATRIX rotY = dx::XMMatrixRotationY(mRotation.y);
    dx::XMMATRIX rotZ = dx::XMMatrixRotationZ(mRotation.z);

    // 计算缩放矩阵
    dx::XMMATRIX scaleMatrix = dx::XMMatrixScaling(mScale.x, mScale.y, mScale.z);

    // 计算平移矩阵
    dx::XMMATRIX translationMatrix = dx::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

    // 组合变换矩阵：缩放 -> 旋转 -> 平移
    mWorldMatrix = scaleMatrix * rotX * rotY * rotZ * translationMatrix;
}