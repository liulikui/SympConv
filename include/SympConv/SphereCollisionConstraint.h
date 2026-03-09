#ifndef SYMPCONV_SPHERE_COLLISION_CONSTRAINT_H
#define SYMPCONV_SPHERE_COLLISION_CONSTRAINT_H

#include <DirectXMath.h>
#include "Constraint.h"
#include "Particle.h"

namespace SympConv {

namespace dx = DirectX;

class SphereCollisionConstraint : public Constraint
{
public:
    SphereCollisionConstraint(Particle* p, const dx::XMFLOAT3& center, float radius, float compliance, float damping)
        : Constraint(compliance, damping)
        , mParticle(p)
        , mSphereCenter(center)
        , mSphereRadius(radius)
    {
    }

    float ComputeConstraintAndGradient(dx::XMFLOAT3* gradients) const override
    {
        if (mParticle->mIsStatic)
        {
            gradients[0] = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
            return 0.0f;
        }
        else
        {
            dx::XMVECTOR pos = dx::XMLoadFloat3(&mParticle->mPosition);
            dx::XMVECTOR center = dx::XMLoadFloat3(&mSphereCenter);
            dx::XMVECTOR toCenter = dx::XMVectorSubtract(pos, center);
            float distance = dx::XMVectorGetX(dx::XMVector3Length(toCenter));

            if (distance > mSphereRadius)
            {
                gradients[0] = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);

                return 0.0f;
            }
            else
            {
                if (distance > 1e-6f)
                {
                    dx::XMVECTOR gradient = dx::XMVectorScale(toCenter, 1.0f / distance);
                    dx::XMFLOAT3 gradientFloat3;
                    dx::XMStoreFloat3(&gradientFloat3, gradient);

                    gradients[0] = gradientFloat3;

                    return distance - mSphereRadius;
                }
                else
                {
                    gradients[0] = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);

                    return 0.0f;
                }
            }
        }
    }

    virtual uint32_t GetParticlesCount() const override
    {
        return 1;
    }

    virtual Particle** GetParticles()
    {
        return &mParticle;
    }

    virtual const Particle** GetParticles() const override
    {
        return (const Particle**)(&mParticle);
    }

    virtual const char* GetConstraintType() const override
    {
        return "SphereCollision";
    }

private:
    Particle* mParticle;
    dx::XMFLOAT3 mSphereCenter;
    float mSphereRadius;
};

}
#endif // SYMPCONV_SPHERE_COLLISION_CONSTRAINT_H
