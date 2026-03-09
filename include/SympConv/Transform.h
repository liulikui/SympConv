#ifndef SYMPCONV_TRANSFORM_H
#define SYMPCONV_TRANSFORM_H

#include "Config.h"
#include "Quaternion.h"
#include "Vector.h"
#include "Matrix.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 变换模板类
 * @details 表示三维空间中的变换，包括平移、旋转和缩放
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TTransform
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mTranslation;  ///< 平移向量
    TVector3<T> mScale;         ///< 缩放向量
    TQuaternion<T> mOrientation; ///< 旋转四元数

public:
    /**
     * @brief 默认构造函数
     * @details 初始化为单位变换（零平移、单位缩放、单位四元数）
     */
    TTransform() : 
        mTranslation(T(0), T(0), T(0)), 
        mScale(T(1), T(1), T(1)), 
        mOrientation() {}

    /**
     * @brief 带参数的构造函数
     * @param translation 平移向量
     * @param scale 缩放向量
     * @param orientation 旋转四元数
     */
    TTransform(const TVector3<T>& translation, const TVector3<T>& scale, const TQuaternion<T>& orientation) :
        mTranslation(translation), 
        mScale(scale), 
        mOrientation(orientation) {}

    /**
     * @brief 从平移和旋转创建变换
     * @param translation 平移向量
     * @param orientation 旋转四元数
     */
    TTransform(const TVector3<T>& translation, const TQuaternion<T>& orientation) :
        mTranslation(translation), 
        mScale(T(1), T(1), T(1)), 
        mOrientation(orientation) {}

    /**
     * @brief 从平移创建变换
     * @param translation 平移向量
     */
    TTransform(const TVector3<T>& translation) :
        mTranslation(translation), 
        mScale(T(1), T(1), T(1)), 
        mOrientation() {}

    /**
     * @brief 从四元数创建变换
     * @param orientation 旋转四元数
     */
    TTransform(const TQuaternion<T>& orientation) :
        mTranslation(T(0), T(0), T(0)), 
        mScale(T(1), T(1), T(1)), 
        mOrientation(orientation) {}

    /**
     * @brief 复制构造函数
     * @param other 另一个变换
     */
    TTransform(const TTransform<T>& other) = default;

    /**
     * @brief 移动构造函数
     * @param other 另一个变换
     */
    TTransform(TTransform<T>&& other) = default;

    /**
     * @brief 赋值运算符
     * @param other 另一个变换
     * @return 引用到当前变换
     */
    TTransform<T>& operator=(const TTransform<T>& other) = default;

    /**
     * @brief 移动赋值运算符
     * @param other 另一个变换
     * @return 引用到当前变换
     */
    TTransform<T>& operator=(TTransform<T>&& other) = default;

    /**
     * @brief 变换向量
     * @param vec 要变换的向量
     * @return 变换后的向量
     */
    TVector3<T> TransformVector(const TVector3<T>& vec) const
    {
        // 缩放 -> 旋转（不包含平移）
        return mOrientation.RotateVector(vec * mScale);
    }

    /**
     * @brief 变换点
     * @param point 要变换的点
     * @return 变换后的点
     */
    TVector3<T> TransformPoint(const TVector3<T>& point) const
    {
        // 缩放 -> 旋转 -> 平移
        return mOrientation.RotateVector(point * mScale) + mTranslation;
    }

    /**
     * @brief 变换方向向量（不包含平移）
     * @param dir 要变换的方向向量
     * @return 变换后的方向向量
     */
    TVector3<T> TransformDirection(const TVector3<T>& dir) const
    {
        // 缩放 -> 旋转（不包含平移）
        return mOrientation.RotateVector(dir * mScale);
    }

    /**
     * @brief 反向变换向量
     * @param vec 要反向变换的向量
     * @return 反向变换后的向量
     */
    TVector3<T> InverseTransformVector(const TVector3<T>& vec) const
    {
        // 反向旋转 -> 反向缩放（不包含平移）
        TVector3<T> temp = mOrientation.Inverse().RotateVector(vec);
        return temp / mScale;
    }

    /**
     * @brief 反向变换点
     * @param point 要反向变换的点
     * @return 反向变换后的点
     */
    TVector3<T> InverseTransformPoint(const TVector3<T>& point) const
    {
        // 反向平移 -> 反向旋转 -> 反向缩放
        TVector3<T> temp = point - mTranslation;
        temp = mOrientation.Inverse().RotateVector(temp);
        return temp / mScale;
    }

    /**
     * @brief 反向变换方向向量（不包含平移）
     * @param dir 要反向变换的方向向量
     * @return 反向变换后的方向向量
     */
    TVector3<T> InverseTransformDirection(const TVector3<T>& dir) const
    {
        // 反向旋转 -> 反向缩放（不包含平移）
        TVector3<T> temp = mOrientation.Inverse().RotateVector(dir);
        return temp / mScale;
    }

    /**
     * @brief 转换为变换矩阵（4x4）
     * @return 4x4变换矩阵
     */
    TMatrix4x4<T> ToMatrix4x4() const
    {
        // 计算旋转缩放矩阵
        TMatrix3x3<T> rotScaleMat = mOrientation.ToMatrix3x3();
        
        // 应用缩放
        TMatrix3x3<T> scaleMat(
            mScale.x, 0, 0,
            0, mScale.y, 0,
            0, 0, mScale.z
        );
        TMatrix3x3<T> rotScale = rotScaleMat * scaleMat;

        // 构建4x4矩阵
        return TMatrix4x4<T>(
            rotScale.mRows[0].x, rotScale.mRows[0].y, rotScale.mRows[0].z, mTranslation.x,
            rotScale.mRows[1].x, rotScale.mRows[1].y, rotScale.mRows[1].z, mTranslation.y,
            rotScale.mRows[2].x, rotScale.mRows[2].y, rotScale.mRows[2].z, mTranslation.z,
            0, 0, 0, 1
        );
    }

    /**
     * @brief 从变换矩阵创建变换
     * @param mat 4x4变换矩阵
     * @return 对应的变换
     */
    static TTransform<T> FromMatrix4x4(const TMatrix4x4<T>& mat)
    {
        TTransform<T> transform;
        
        // 提取平移部分
        transform.mTranslation = TVector3<T>(mat.mRows[0].w, mat.mRows[1].w, mat.mRows[2].w);
        
        // 提取旋转缩放部分
        TMatrix3x3<T> rotScaleMat(
            mat.mRows[0].x, mat.mRows[0].y, mat.mRows[0].z,
            mat.mRows[1].x, mat.mRows[1].y, mat.mRows[1].z,
            mat.mRows[2].x, mat.mRows[2].y, mat.mRows[2].z
        );
        
        // 计算缩放因子
        TVector3<T> scale;
        scale.x = TVector3<T>(rotScaleMat.mRows[0].x, rotScaleMat.mRows[0].y, rotScaleMat.mRows[0].z).Length();
        scale.y = TVector3<T>(rotScaleMat.mRows[1].x, rotScaleMat.mRows[1].y, rotScaleMat.mRows[1].z).Length();
        scale.z = TVector3<T>(rotScaleMat.mRows[2].x, rotScaleMat.mRows[2].y, rotScaleMat.mRows[2].z).Length();
        
        // 确保缩放因子不为零
        if (scale.x < T(1e-6)) scale.x = T(1);
        if (scale.y < T(1e-6)) scale.y = T(1);
        if (scale.z < T(1e-6)) scale.z = T(1);
        
        transform.mScale = scale;
        
        // 计算旋转矩阵（去除缩放）
        TMatrix3x3<T> rotMat = rotScaleMat;
        rotMat.mRows[0] /= scale.x;
        rotMat.mRows[1] /= scale.y;
        rotMat.mRows[2] /= scale.z;
        
        // 从旋转矩阵创建四元数
        transform.mOrientation = TQuaternion<T>::FromMatrix3x3(rotMat);
        
        return transform;
    }

    /**
     * @brief 组合变换
     * @param other 另一个变换
     * @return 组合后的变换
     */
    TTransform<T> operator*(const TTransform<T>& other) const
    {
        TTransform<T> result;
        
        // 组合缩放
        result.mScale = mScale * other.mScale;
        
        // 组合旋转
        result.mOrientation = mOrientation * other.mOrientation;
        
        // 组合平移
        result.mTranslation = mTranslation + mOrientation.RotateVector(other.mTranslation * mScale);
        
        return result;
    }

    /**
     * @brief 组合变换复合赋值运算符
     * @param other 另一个变换
     * @return 引用到当前变换
     */
    TTransform<T>& operator*=(const TTransform<T>& other)
    {
        *this = *this * other;
        return *this;
    }

    /**
     * @brief 平移变换
     * @param delta 平移量
     * @return 平移后的变换
     */
    TTransform<T> Translate(const TVector3<T>& delta) const
    {
        TTransform<T> result = *this;
        result.mTranslation += delta;
        return result;
    }

    /**
     * @brief 平移变换（修改当前变换）
     * @param delta 平移量
     * @return 引用到当前变换
     */
    TTransform<T>& TranslateSelf(const TVector3<T>& delta)
    {
        mTranslation += delta;
        return *this;
    }

    /**
     * @brief 旋转变换
     * @param rotation 旋转四元数
     * @return 旋转后的变换
     */
    TTransform<T> Rotate(const TQuaternion<T>& rotation) const
    {
        TTransform<T> result = *this;
        result.mOrientation = rotation * result.mOrientation;
        return result;
    }

    /**
     * @brief 旋转变换（修改当前变换）
     * @param rotation 旋转四元数
     * @return 引用到当前变换
     */
    TTransform<T>& RotateSelf(const TQuaternion<T>& rotation)
    {
        mOrientation = rotation * mOrientation;
        return *this;
    }

    /**
     * @brief 缩放变换
     * @param scale 缩放因子
     * @return 缩放后的变换
     */
    TTransform<T> Scale(const TVector3<T>& scale) const
    {
        TTransform<T> result = *this;
        result.mScale *= scale;
        return result;
    }

    /**
     * @brief 缩放变换（修改当前变换）
     * @param scale 缩放因子
     * @return 引用到当前变换
     */
    TTransform<T>& ScaleSelf(const TVector3<T>& scale)
    {
        mScale *= scale;
        return *this;
    }

    /**
     * @brief 计算逆变换
     * @return 逆变换
     */
    TTransform<T> Inverse() const
    {
        TTransform<T> result;
        
        // 计算逆旋转
        result.mOrientation = mOrientation.Inverse();
        
        // 计算逆缩放
        result.mScale = TVector3<T>(T(1) / mScale.x, T(1) / mScale.y, T(1) / mScale.z);
        
        // 计算逆平移
        result.mTranslation = -result.mOrientation.RotateVector(mTranslation * result.mScale);
        
        return result;
    }

    /**
     * @brief 线性插值
     * @param other 目标变换
     * @param t 插值参数 [0, 1]
     * @return 插值结果
     */
    TTransform<T> Lerp(const TTransform<T>& other, T t) const
    {
        TTransform<T> result;
        
        // 插值平移
        result.mTranslation = mTranslation + (other.mTranslation - mTranslation) * t;
        
        // 插值缩放
        result.mScale = mScale + (other.mScale - mScale) * t;
        
        // 插值旋转（球面线性插值）
        result.mOrientation = TQuaternion<T>::Slerp(mOrientation, other.mOrientation, t);
        
        return result;
    }

    /**
     * @brief 静态线性插值
     * @param a 起始变换
     * @param b 目标变换
     * @param t 插值参数 [0, 1]
     * @return 插值结果
     */
    static TTransform<T> Lerp(const TTransform<T>& a, const TTransform<T>& b, T t)
    {
        return a.Lerp(b, t);
    }

    /**
     * @brief 获取前向向量
     * @return 前向向量
     */
    TVector3<T> GetForward() const
    {
        return mOrientation.RotateVector(TVector3<T>(T(0), T(0), T(1)));
    }

    /**
     * @brief 获取右向向量
     * @return 右向向量
     */
    TVector3<T> GetRight() const
    {
        return mOrientation.RotateVector(TVector3<T>(T(1), T(0), T(0)));
    }

    /**
     * @brief 获取上向向量
     * @return 上向向量
     */
    TVector3<T> GetUp() const
    {
        return mOrientation.RotateVector(TVector3<T>(T(0), T(1), T(0)));
    }

    /**
     * @brief 设置前向向量（保持向上方向）
     * @param forward 前向向量
     * @return 引用到当前变换
     */
    TTransform<T>& SetForward(const TVector3<T>& forward)
    {
        TVector3<T> up = GetUp();
        TVector3<T> right = forward.Cross(up).Normalize();
        up = right.Cross(forward).Normalize();
        
        // 构建旋转矩阵
        TMatrix3x3<T> rotMat(
            right.x, up.x, forward.x,
            right.y, up.y, forward.y,
            right.z, up.z, forward.z
        );
        
        mOrientation = TQuaternion<T>::FromMatrix3x3(rotMat);
        return *this;
    }

    /**
     * @brief 重置为单位变换
     * @return 引用到当前变换
     */
    TTransform<T>& Reset()
    {
        mTranslation = TVector3<T>(T(0), T(0), T(0));
        mScale = TVector3<T>(T(1), T(1), T(1));
        mOrientation = TQuaternion<T>::Identity();
        return *this;
    }

    /**
     * @brief 创建单位变换
     * @return 单位变换
     */
    static TTransform<T> Identity()
    {
        return TTransform<T>();
    }

    /**
     * @brief 创建平移变换
     * @param translation 平移向量
     * @return 平移变换
     */
    static TTransform<T> Translation(const TVector3<T>& translation)
    {
        return TTransform<T>(translation);
    }

    /**
     * @brief 创建旋转变换
     * @param orientation 旋转四元数
     * @return 旋转变换
     */
    static TTransform<T> Rotation(const TQuaternion<T>& orientation)
    {
        return TTransform<T>(orientation);
    }

    /**
     * @brief 创建缩放变换
     * @param scale 缩放向量
     * @return 缩放变换
     */
    static TTransform<T> Scaling(const TVector3<T>& scale)
    {
        TTransform<T> transform;
        transform.mScale = scale;
        return transform;
    }
};

// 类型别名
typedef TTransform<float> Transformf;
typedef TTransform<double> Transformd;
typedef TTransform<fpnumber> Transform;

} // namespace SympConv

#endif // SYMPCONV_TRANSFORM_H
