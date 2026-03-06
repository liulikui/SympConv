#include "Sphere.h"
#include <DirectXMath.h>
#include <cmath>

// 为了方便使用，定义一个简化的命名空间别名
namespace dx = DirectX;

Sphere::Sphere(float radius, uint32_t sectors, uint32_t stacks)
    : mRadius(radius), mSectors(sectors), mStacks(stacks), mCenter(dx::XMFLOAT3(0.0f, 0.0f, 0.0f))
{
    // 生成球体数据
    GenerateSphereData();
}

void Sphere::Update(IRALGraphicsCommandList* commandList, float deltaTime)
{
    // 球体默认不需要复杂的更新逻辑
    // 可以在这里添加动画或其他动态行为
}

bool Sphere::Initialize(IRALDevice* device)
{
    if (!device || mPositions.empty() || mIndices.empty())
    {
        return false;
    }

    return true;
}

void Sphere::OnSetupMesh(IRALDevice* device, PrimitiveMesh& mesh)
{
    // 创建顶点数据（位置 + 法线）
    std::vector<uint8_t> vertexData(mPositions.size() * sizeof(dx::XMFLOAT3) * 2);
    for (size_t i = 0; i < mPositions.size(); ++i)
    {
        size_t positionOffset = i * sizeof(dx::XMFLOAT3) * 2;
        size_t normalOffset = positionOffset + sizeof(dx::XMFLOAT3);

        memcpy(&vertexData[positionOffset], &mPositions[i], sizeof(dx::XMFLOAT3));
        memcpy(&vertexData[normalOffset], &mNormals[i], sizeof(dx::XMFLOAT3));
    }

    // 创建顶点缓冲区
    mesh.mVertexBuffer = device->CreateVertexBuffer(
        vertexData.size(),
        6 * sizeof(float),// 顶点 stride（3个位置分量 + 3个法线分量）
        true,
        vertexData.data(),
		L"SphereVB"
    );

    // 创建索引缓冲区
    mesh.mIndexBuffer = device->CreateIndexBuffer(
        mIndices.size(),
        true,// 32位索引
        true,
        mIndices.data(),
        L"SphereIB"
    );
}

void Sphere::SetRadius(float newRadius)
{
    if (mRadius != newRadius)
    {
        mRadius = newRadius;
        GenerateSphereData();
    }
}

void Sphere::SetCenter(const dx::XMFLOAT3& newCenter)
{
    if (mCenter.x != newCenter.x || mCenter.y != newCenter.y || mCenter.z != newCenter.z)
    {
        mCenter = newCenter;
        SetPosition(mCenter);
    }
}

void Sphere::GenerateSphereData()
{
    // 清空现有数据
    mPositions.clear();
    mNormals.clear();
    mIndices.clear();

    // 生成顶点和法线
    for (uint32_t i = 0; i <= mStacks; ++i)
    {
        float phi = dx::XMConvertToRadians(180.0f * static_cast<float>(i) / mStacks); // 极角
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        for (uint32_t j = 0; j <= mSectors; ++j)
        {
            float theta = dx::XMConvertToRadians(360.0f * static_cast<float>(j) / mSectors); // 方位角
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            // 添加顶点位置
            dx::XMFLOAT3 pos;
            pos.x = mRadius * sinPhi * cosTheta;
            pos.y = mRadius * cosPhi;
            pos.z = mRadius * sinPhi * sinTheta;
            mPositions.push_back(pos);

            // 添加法线（单位向量）
            dx::XMFLOAT3 normal;
            normal.x = sinPhi * cosTheta;
            normal.y = cosPhi;
            normal.z = sinPhi * sinTheta;
            mNormals.push_back(normal);
        }
    }

    // 生成索引
    for (uint32_t i = 0; i < mStacks; ++i)
    {
        uint32_t row1 = i * (mSectors + 1);
        uint32_t row2 = (i + 1) * (mSectors + 1);

        for (uint32_t j = 0; j < mSectors; ++j)
        {
            // 第一个三角形
            mIndices.push_back(row1 + j);
            mIndices.push_back(row2 + j + 1);
            mIndices.push_back(row1 + j + 1);

            // 第二个三角形
            mIndices.push_back(row1 + j);
            mIndices.push_back(row2 + j);
            mIndices.push_back(row2 + j + 1);
        }
    }
}
