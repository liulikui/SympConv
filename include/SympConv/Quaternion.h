#ifndef SYMPCONV_QUATERNION_H
#define SYMPCONV_QUATERNION_H

#include "Config.h"
#include "Vector.h"
#include "Matrix.h"
#include <type_traits>
#include <cmath>

namespace SympConv {

/**
 * @brief 四元数模板类
 * @details 四元数是一种扩展复数，用于表示三维旋转，比欧拉角和旋转矩阵更具优势
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TQuaternion
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T x; ///< 四元数的虚部x分量
    T y; ///< 四元数的虚部y分量
    T z; ///< 四元数的虚部z分量
    T w; ///< 四元数的实部

public:
    /**
     * @brief 默认构造函数
     */
    TQuaternion() : x(T(0)), y(T(0)), z(T(0)), w(T(1)) {}

    /**
     * @brief 带参数的构造函数
     * @param x 虚部x分量
     * @param y 虚部y分量
     * @param z 虚部z分量
     * @param w 实部
     */
    TQuaternion(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    /**
     * @brief 从向量和标量构造四元数
     * @param v 虚部向量
     * @param w 实部
     */
    TQuaternion(const TVector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

    /**
     * @brief 虚部设置函数
     * @param x 虚部x分量
     * @param y 虚部y分量
     * @param z 虚部z分量
     * @return 引用到当前四元数
     */
    TQuaternion<T>& Set(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        return *this;
    }

    /**
     * @brief 四元数加法运算符
     * @param q 另一个四元数
     * @return 两个四元数的和
     */
    TQuaternion<T> operator+(const TQuaternion<T>& q) const
    {
        return TQuaternion<T>(x + q.x, y + q.y, z + q.z, w + q.w);
    }

    /**
     * @brief 四元数减法运算符
     * @param q 另一个四元数
     * @return 两个四元数的差
     */
    TQuaternion<T> operator-(const TQuaternion<T>& q) const
    {
        return TQuaternion<T>(x - q.x, y - q.y, z - q.z, w - q.w);
    }

    /**
     * @brief 四元数乘法运算符（Hamilton积）
     * @param q 另一个四元数
     * @return 两个四元数的乘积
     */
    TQuaternion<T> operator*(const TQuaternion<T>& q) const
    {
        return TQuaternion<T>(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }

    /**
     * @brief 四元数与标量乘法运算符
     * @param scalar 标量值
     * @return 四元数与标量的乘积
     */
    TQuaternion<T> operator*(T scalar) const
    {
        return TQuaternion<T>(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    /**
     * @brief 四元数与标量除法运算符
     * @param scalar 标量值
     * @return 四元数与标量的商
     */
    TQuaternion<T> operator/(T scalar) const
    {
        return TQuaternion<T>(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    /**
     * @brief 四元数加法复合赋值运算符
     * @param q 另一个四元数
     * @return 引用到当前四元数
     */
    TQuaternion<T>& operator+=(const TQuaternion<T>& q)
    {
        x += q.x;
        y += q.y;
        z += q.z;
        w += q.w;
        return *this;
    }

    /**
     * @brief 四元数减法复合赋值运算符
     * @param q 另一个四元数
     * @return 引用到当前四元数
     */
    TQuaternion<T>& operator-=(const TQuaternion<T>& q)
    {
        x -= q.x;
        y -= q.y;
        z -= q.z;
        w -= q.w;
        return *this;
    }

    /**
     * @brief 四元数乘法复合赋值运算符
     * @param q 另一个四元数
     * @return 引用到当前四元数
     */
    TQuaternion<T>& operator*=(const TQuaternion<T>& q)
    {
        *this = *this * q;
        return *this;
    }

    /**
     * @brief 标量乘法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前四元数
     */
    TQuaternion<T>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    /**
     * @brief 标量除法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前四元数
     */
    TQuaternion<T>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    /**
     * @brief 四元数点积
     * @param q 另一个四元数
     * @return 点积结果
     */
    T Dot(const TQuaternion<T>& q) const
    {
        return x * q.x + y * q.y + z * q.z + w * q.w;
    }

    /**
     * @brief 静态四元数点积
     * @param a 第一个四元数
     * @param b 第二个四元数
     * @return 点积结果
     */
    static T DotProduct(const TQuaternion<T>& a, const TQuaternion<T>& b)
    {
        return a.Dot(b);
    }

    /**
     * @brief 获取四元数的虚部向量
     * @return 虚部向量
     */
    TVector3<T> GetImaginary() const
    {
        return TVector3<T>(x, y, z);
    }

    /**
     * @brief 计算四元数的模（长度）
     * @return 四元数的模
     */
    T Length() const
    {
        return std::sqrt(LengthSquared());
    }

    /**
     * @brief 计算四元数模的平方
     * @return 四元数模的平方
     */
    T LengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    /**
     * @brief 归一化四元数
     * @return 归一化后的四元数
     */
    TQuaternion<T> Normalize() const
    {
        T len = Length();
        if (len > T(0))
        {
            T invLen = T(1) / len;
            return TQuaternion<T>(x * invLen, y * invLen, z * invLen, w * invLen);
        }
        return TQuaternion<T>::Identity();
    }

    /**
     * @brief 四元数共轭
     * @return 共轭四元数
     */
    TQuaternion<T> Conjugate() const
    {
        return TQuaternion<T>(-x, -y, -z, w);
    }

    /**
     * @brief 四元数逆
     * @return 逆四元数
     */
    TQuaternion<T> Inverse() const
    {
        T lenSq = LengthSquared();
        if (lenSq > T(0))
        {
            T invLenSq = T(1) / lenSq;
            return TQuaternion<T>(-x * invLenSq, -y * invLenSq, -z * invLenSq, w * invLenSq);
        }
        return TQuaternion<T>::Identity();
    }

    /**
     * @brief 四元数幂运算
     * @param exponent 指数
     * @return 幂运算结果
     */
    TQuaternion<T> Pow(T exponent) const
    {
        if (LengthSquared() == T(0))
        {
            return TQuaternion<T>::Identity();
        }

        T theta = std::acos(w);
        T sinTheta = std::sin(theta);

        if (std::abs(sinTheta) < T(1e-6))
        {
            return *this;
        }

        T newTheta = exponent * theta;
        T sinNewTheta = std::sin(newTheta);
        T cosNewTheta = std::cos(newTheta);

        return TQuaternion<T>(
            x * sinNewTheta / sinTheta,
            y * sinNewTheta / sinTheta,
            z * sinNewTheta / sinTheta,
            cosNewTheta
        );
    }

    /**
     * @brief 球面线性插值
     * @param target 目标四元数
     * @param t 插值参数 [0, 1]
     * @return 插值结果四元数
     */
    TQuaternion<T> Slerp(const TQuaternion<T>& target, T t) const
    {
        T dot = Dot(target);

        TQuaternion<T> q = target;
        if (dot < T(0))
        {
            q = TQuaternion<T>(-target.x, -target.y, -target.z, -target.w);
            dot = -dot;
        }

        if (dot > T(1) - T(1e-6))
        {
            return TQuaternion<T>(
                x + t * (q.x - x),
                y + t * (q.y - y),
                z + t * (q.z - z),
                w + t * (q.w - w)
            ).Normalize();
        }

        T theta0 = std::acos(dot);
        T theta = theta0 * t;
        T sinTheta = std::sin(theta);
        T sinTheta0 = std::sin(theta0);

        T s0 = std::cos(theta) - dot * sinTheta / sinTheta0;
        T s1 = sinTheta / sinTheta0;

        return TQuaternion<T>(
            x * s0 + q.x * s1,
            y * s0 + q.y * s1,
            z * s0 + q.z * s1,
            w * s0 + q.w * s1
        );
    }

    /**
     * @brief 静态球面线性插值
     * @param a 起始四元数
     * @param b 目标四元数
     * @param t 插值参数 [0, 1]
     * @return 插值结果四元数
     */
    static TQuaternion<T> Slerp(const TQuaternion<T>& a, const TQuaternion<T>& b, T t)
    {
        return a.Slerp(b, t);
    }

    /**
     * @brief 使用四元数旋转向量
     * @param vec 要旋转的向量
     * @return 旋转后的向量
     */
    TVector3<T> RotateVector(const TVector3<T>& vec) const
    {
        TQuaternion<T> qv(vec.x, vec.y, vec.z, T(0));
        TQuaternion<T> result = *this * qv * Inverse();
        return result.GetImaginary();
    }

    /**
     * @brief 静态旋转向量函数
     * @param q 用于旋转的四元数
     * @param vec 要旋转的向量
     * @return 旋转后的向量
     */
    static TVector3<T> RotateVector(const TQuaternion<T>& q, const TVector3<T>& vec)
    {
        return q.RotateVector(vec);
    }

    /**
     * @brief 转换为旋转矩阵（3x3）
     * @return 3x3旋转矩阵
     */
    TMatrix3x3<T> ToMatrix3x3() const
    {
        T xx = x * x;
        T yy = y * y;
        T zz = z * z;
        T xy = x * y;
        T xz = x * z;
        T yz = y * z;
        T wx = w * x;
        T wy = w * y;
        T wz = w * z;

        return TMatrix3x3<T>(
            T(1) - T(2) * (yy + zz), T(2) * (xy + wz),        T(2) * (xz - wy),
            T(2) * (xy - wz),        T(1) - T(2) * (xx + zz), T(2) * (yz + wx),
            T(2) * (xz + wy),        T(2) * (yz - wx),        T(1) - T(2) * (xx + yy)
        );
    }

    /**
     * @brief 转换为旋转矩阵（4x4）
     * @return 4x4旋转矩阵
     */
    TMatrix4x4<T> ToMatrix4x4() const
    {
        TMatrix3x3<T> m3 = ToMatrix3x3();
        return TMatrix4x4<T>(
            m3.mRows[0].x, m3.mRows[0].y, m3.mRows[0].z, T(0),
            m3.mRows[1].x, m3.mRows[1].y, m3.mRows[1].z, T(0),
            m3.mRows[2].x, m3.mRows[2].y, m3.mRows[2].z, T(0),
            T(0),          T(0),          T(0),          T(1)
        );
    }

    /**
     * @brief 从旋转矩阵创建四元数
     * @param mat 3x3旋转矩阵
     * @return 对应的四元数
     */
    static TQuaternion<T> FromMatrix3x3(const TMatrix3x3<T>& mat)
    {
        T trace = mat.mRows[0].x + mat.mRows[1].y + mat.mRows[2].z;

        if (trace > T(0))
        {
            T s = T(0.5) / std::sqrt(trace + T(1));
            return TQuaternion<T>(
                (mat.mRows[2].y - mat.mRows[1].z) * s,
                (mat.mRows[0].z - mat.mRows[2].x) * s,
                (mat.mRows[1].x - mat.mRows[0].y) * s,
                T(0.25) / s
            );
        }
        else if (mat.mRows[0].x > mat.mRows[1].y && mat.mRows[0].x > mat.mRows[2].z)
        {
            T s = T(2) * std::sqrt(T(1) + mat.mRows[0].x - mat.mRows[1].y - mat.mRows[2].z);
            return TQuaternion<T>(
                T(0.25) * s,
                (mat.mRows[0].y + mat.mRows[1].x) / s,
                (mat.mRows[0].z + mat.mRows[2].x) / s,
                (mat.mRows[2].y - mat.mRows[1].z) / s
            );
        }
        else if (mat.mRows[1].y > mat.mRows[2].z)
        {
            T s = T(2) * std::sqrt(T(1) + mat.mRows[1].y - mat.mRows[0].x - mat.mRows[2].z);
            return TQuaternion<T>(
                (mat.mRows[0].y + mat.mRows[1].x) / s,
                T(0.25) * s,
                (mat.mRows[1].z + mat.mRows[2].y) / s,
                (mat.mRows[0].z - mat.mRows[2].x) / s
            );
        }
        else
        {
            T s = T(2) * std::sqrt(T(1) + mat.mRows[2].z - mat.mRows[0].x - mat.mRows[1].y);
            return TQuaternion<T>(
                (mat.mRows[0].z + mat.mRows[2].x) / s,
                (mat.mRows[1].z + mat.mRows[2].y) / s,
                T(0.25) * s,
                (mat.mRows[1].x - mat.mRows[0].y) / s
            );
        }
    }

    /**
     * @brief 从旋转矩阵创建四元数（4x4版本）
     * @param mat 4x4旋转矩阵
     * @return 对应的四元数
     */
    static TQuaternion<T> FromMatrix4x4(const TMatrix4x4<T>& mat)
    {
        TMatrix3x3<T> m3(
            mat.mRows[0].x, mat.mRows[0].y, mat.mRows[0].z,
            mat.mRows[1].x, mat.mRows[1].y, mat.mRows[1].z,
            mat.mRows[2].x, mat.mRows[2].y, mat.mRows[2].z
        );
        return FromMatrix3x3(m3);
    }

    /**
     * @brief 从轴角创建四元数
     * @param axis 旋转轴（单位向量）
     * @param angle 旋转角度（弧度）
     * @return 对应的四元数
     */
    static TQuaternion<T> FromAxisAngle(const TVector3<T>& axis, T angle)
    {
        T halfAngle = angle * T(0.5);
        T sinHalfAngle = std::sin(halfAngle);
        T cosHalfAngle = std::cos(halfAngle);

        return TQuaternion<T>(
            axis.x * sinHalfAngle,
            axis.y * sinHalfAngle,
            axis.z * sinHalfAngle,
            cosHalfAngle
        );
    }

    /**
     * @brief 转换为轴角表示
     * @param axis 输出旋转轴
     * @return 输出旋转角度（弧度）
     */
    T ToAxisAngle(TVector3<T>& axis) const
    {
        T qw = w;
        if (qw < T(0))
        {
            qw = -qw;
        }

        T angle = T(2) * std::acos(qw);
        T sinHalfAngle = std::sqrt(T(1) - qw * qw);

        if (sinHalfAngle > T(1e-6))
        {
            axis.x = x / sinHalfAngle;
            axis.y = y / sinHalfAngle;
            axis.z = z / sinHalfAngle;
        }
        else
        {
            axis.x = T(1);
            axis.y = T(0);
            axis.z = T(0);
        }

        return angle;
    }

    /**
     * @brief 从欧拉角创建四元数（ZYX顺序，即Yaw-Pitch-Roll）
     * @param x 绕X轴旋转角度（弧度）
     * @param y 绕Y轴旋转角度（弧度）
     * @param z 绕Z轴旋转角度（弧度）
     * @return 对应的四元数
     */
    static TQuaternion<T> FromEulerAngles(T x, T y, T z)
    {
        T halfX = x * T(0.5);
        T halfY = y * T(0.5);
        T halfZ = z * T(0.5);

        T sinX = std::sin(halfX);
        T cosX = std::cos(halfX);
        T sinY = std::sin(halfY);
        T cosY = std::cos(halfY);
        T sinZ = std::sin(halfZ);
        T cosZ = std::cos(halfZ);

        return TQuaternion<T>(
            sinX * cosY * cosZ - cosX * sinY * sinZ,
            cosX * sinY * cosZ + sinX * cosY * sinZ,
            cosX * cosY * sinZ - sinX * sinY * cosZ,
            cosX * cosY * cosZ + sinX * sinY * sinZ
        );
    }

    /**
     * @brief 转换为欧拉角（ZYX顺序）
     * @param outX 输出绕X轴旋转角度（弧度）
     * @param outY 输出绕Y轴旋转角度（弧度）
     * @param outZ 输出绕Z轴旋转角度（弧度）
     */
    void ToEulerAngles(T& outX, T& outY, T& outZ) const
    {
        T sinrCosp = T(2) * (w * x + y * z);
        T cosrCosp = T(1) - T(2) * (x * x + y * y);
        outZ = std::atan2(sinrCosp, cosrCosp);

        T sinp = T(2) * (w * y - z * x);
        if (std::abs(sinp) >= T(1))
        {
            outY = std::copysign(T(3.14159265358979323846) / T(2), sinp);
        }
        else
        {
            outY = std::asin(sinp);
        }

        T sinyCosp = T(2) * (w * z + x * y);
        T cosyCosp = T(1) - T(2) * (y * y + z * z);
        outX = std::atan2(sinyCosp, cosyCosp);
    }

    /**
     * @brief 从欧拉角创建四元数（重载版本）
     * @param euler 欧拉角向量（弧度），按XYZ顺序
     * @return 对应的四元数
     */
    static TQuaternion<T> FromEulerAngles(const TVector3<T>& euler)
    {
        return FromEulerAngles(euler.x, euler.y, euler.z);
    }

    /**
     * @brief 转换为欧拉角
     * @return 欧拉角向量（弧度），按XYZ顺序
     */
    TVector3<T> ToEulerAngles() const
    {
        T x, y, z;
        ToEulerAngles(x, y, z);
        return TVector3<T>(x, y, z);
    }

    /**
     * @brief 绕X轴旋转
     * @param angle 旋转角度（弧度）
     * @return 旋转后的四元数
     */
    static TQuaternion<T> RotateX(T angle)
    {
        return FromAxisAngle(TVector3<T>(T(1), T(0), T(0)), angle);
    }

    /**
     * @brief 绕Y轴旋转
     * @param angle 旋转角度（弧度）
     * @return 旋转后的四元数
     */
    static TQuaternion<T> RotateY(T angle)
    {
        return FromAxisAngle(TVector3<T>(T(0), T(1), T(0)), angle);
    }

    /**
     * @brief 绕Z轴旋转
     * @param angle 旋转角度（弧度）
     * @return 旋转后的四元数
     */
    static TQuaternion<T> RotateZ(T angle)
    {
        return FromAxisAngle(TVector3<T>(T(0), T(0), T(1)), angle);
    }

    /**
     * @brief 绕任意轴旋转
     * @param axis 旋转轴（单位向量）
     * @param angle 旋转角度（弧度）
     * @return 旋转后的四元数
     */
    static TQuaternion<T> RotateAxis(const TVector3<T>& axis, T angle)
    {
        return FromAxisAngle(axis, angle);
    }

    /**
     * @brief 创建单位四元数（无旋转）
     * @return 单位四元数
     */
    static TQuaternion<T> Identity()
    {
        return TQuaternion<T>(T(0), T(0), T(0), T(1));
    }

    /**
     * @brief 创建零四元数
     * @return 零四元数
     */
    static TQuaternion<T> Zero()
    {
        return TQuaternion<T>(T(0), T(0), T(0), T(0));
    }

    /**
     * @brief 查找最小角度的四元数（用于插值）
     * @param q 另一个四元数
     * @return 与原四元数夹角较小的那个四元数
     */
    const TQuaternion<T>& MinimizeAngle(const TQuaternion<T>& q) const
    {
        if (Dot(q) < T(0))
        {
            return q;
        }
        return q;
    }
};

template<typename T>
TQuaternion<T> operator*(T scalar, const TQuaternion<T>& q)
{
    return q * scalar;
}

// 类型别名
typedef TQuaternion<float> Quaternionf;
typedef TQuaternion<double> Quaterniond;
typedef TQuaternion<fpnumber> Quaternion;

} // namespace SympConv

#endif // SYMPCONV_QUATERNION_H
