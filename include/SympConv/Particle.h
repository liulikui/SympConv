#ifndef SYMPCONV_PARTICLE_H
#define SYMPCONV_PARTICLE_H

#include <DirectXMath.h>

// 为了方便使用，定义一个简化的命名空间别名
namespace dx = DirectX;

class Particle
{
public:
    // 构造函数
    // 参数：
    //   pos - 粒子的初始位置
    //   m - 粒子的质量
    //   isStatic - 粒子是否为固定（不可移动）
    Particle(const dx::XMFLOAT3& pos = dx::XMFLOAT3(0.0f, 0.0f, 0.0f), float m = 1.0f, bool isStatic = false)
        : mPosition(pos)
        , mOldPosition(pos)
        , mVelocity(dx::XMFLOAT3(0.0f, 0.0f, 0.0f))
        , mForce(dx::XMFLOAT3(0.0f, 0.0f, 0.0f))
        , mMass(m)
        , mInverseMass(1.0f / m)
        , mIsStatic(isStatic)
    {
        if (isStatic)
        {
            mInverseMass = 0.0f; // 固定粒子的质量倒数为0，表示不受力
        }
    }

    // 应用力
    // 参数：
    //   f - 要应用的力向量
    void ApplyForce(const dx::XMFLOAT3& f)
    {
        if (!mIsStatic) 
        {
            // 将力转换为XMVECTOR进行计算
            dx::XMVECTOR forceVector = dx::XMLoadFloat3(&mForce);
            dx::XMVECTOR appliedForce = dx::XMLoadFloat3(&f);
            
            // 叠加力
            forceVector = dx::XMVectorAdd(forceVector, appliedForce);
            
            // 将结果转换回XMFLOAT3
            dx::XMStoreFloat3(&mForce, forceVector);
        }
    }

    // 重置力
    void ResetForce()
    {
        mForce = dx::XMFLOAT3(0.0f, 0.0f, 0.0f);
    }

    // 公共成员变量
    dx::XMFLOAT3 mPosition;              // 当前位置
    dx::XMFLOAT3 mPositionInitial;       // 单帧初始位置（用于计算帧末总速度）
    dx::XMFLOAT3 mOldPosition;           // 迭代前的位置
    dx::XMFLOAT3 mPredPosition;          // 预测位置
    dx::XMFLOAT3 mVelocity;              // 速度
    dx::XMFLOAT3 mForce;                 // 作用在粒子上的力
    float mMass;                         // 质量
    float mInverseMass;                  // 质量的倒数（用于加速度计算）
    bool mIsStatic;                      // 是否为固定粒子
#ifdef DEBUG_SOLVER
    int mCoordW;
    int mCoordH;
#endif//DEBUG_SOLVER
};

#endif // SYMPCONV_PARTICLE_H
