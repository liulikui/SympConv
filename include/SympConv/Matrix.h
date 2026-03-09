#ifndef SYMPCONV_MATRIX_H
#define SYMPCONV_MATRIX_H

#include "Vector.h"
#include <cmath>

namespace SympConv {

/**
 * @brief 2x2矩阵模板类
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct Matrix2x2
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector2<T> mRows[2]; ///< 矩阵的行向量

public:
    /**
     * @brief 默认构造函数
     */
    Matrix2x2() = default;

    /**
     * @brief 带参数的构造函数
     * @param m00 第一行第一列元素
     * @param m01 第一行第二列元素
     * @param m10 第二行第一列元素
     * @param m11 第二行第二列元素
     */
    Matrix2x2(T m00, T m01, T m10, T m11)
    {
        mRows[0] = TVector2<T>(m00, m01);
        mRows[1] = TVector2<T>(m10, m11);
    }

    /**
     * @brief 矩阵加法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的和
     */
    Matrix2x2<T> operator+(const Matrix2x2<T>& mat) const
    {
        Matrix2x2<T> result;
        result.mRows[0] = mRows[0] + mat.mRows[0];
        result.mRows[1] = mRows[1] + mat.mRows[1];
        return result;
    }

    /**
     * @brief 矩阵减法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的差
     */
    Matrix2x2<T> operator-(const Matrix2x2<T>& mat) const
    {
        Matrix2x2<T> result;
        result.mRows[0] = mRows[0] - mat.mRows[0];
        result.mRows[1] = mRows[1] - mat.mRows[1];
        return result;
    }

    /**
     * @brief 矩阵乘法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的乘积
     */
    Matrix2x2<T> operator*(const Matrix2x2<T>& mat) const
    {
        Matrix2x2<T> result;
        result.mRows[0].x = mRows[0].x * mat.mRows[0].x + mRows[0].y * mat.mRows[1].x;
        result.mRows[0].y = mRows[0].x * mat.mRows[0].y + mRows[0].y * mat.mRows[1].y;
        result.mRows[1].x = mRows[1].x * mat.mRows[0].x + mRows[1].y * mat.mRows[1].x;
        result.mRows[1].y = mRows[1].x * mat.mRows[0].y + mRows[1].y * mat.mRows[1].y;
        return result;
    }

    /**
     * @brief 矩阵与向量乘法
     * @param vec 向量
     * @return 乘积结果向量
     */
    TVector2<T> operator*(const TVector2<T>& vec) const
    {
        TVector2<T> result;
        result.x = mRows[0].x * vec.x + mRows[0].y * vec.y;
        result.y = mRows[1].x * vec.x + mRows[1].y * vec.y;
        return result;
    }

    /**
     * @brief 标量乘法运算符
     * @param scalar 标量值
     * @return 矩阵与标量的乘积
     */
    Matrix2x2<T> operator*(T scalar) const
    {
        Matrix2x2<T> result;
        result.mRows[0] = mRows[0] * scalar;
        result.mRows[1] = mRows[1] * scalar;
        return result;
    }

    /**
     * @brief 标量除法运算符
     * @param scalar 标量值
     * @return 矩阵与标量的商
     */
    Matrix2x2<T> operator/(T scalar) const
    {
        Matrix2x2<T> result;
        result.mRows[0] = mRows[0] / scalar;
        result.mRows[1] = mRows[1] / scalar;
        return result;
    }

    /**
     * @brief 矩阵加法复合赋值运算符
     * @param mat 另一个矩阵
     * @return 引用到当前矩阵
     */
    Matrix2x2<T>& operator+=(const Matrix2x2<T>& mat)
    {
        mRows[0] += mat.mRows[0];
        mRows[1] += mat.mRows[1];
        return *this;
    }

    /**
     * @brief 矩阵减法复合赋值运算符
     * @param mat 另一个矩阵
     * @return 引用到当前矩阵
     */
    Matrix2x2<T>& operator-=(const Matrix2x2<T>& mat)
    {
        mRows[0] -= mat.mRows[0];
        mRows[1] -= mat.mRows[1];
        return *this;
    }

    /**
     * @brief 标量乘法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前矩阵
     */
    Matrix2x2<T>& operator*=(T scalar)
    {
        mRows[0] *= scalar;
        mRows[1] *= scalar;
        return *this;
    }

    /**
     * @brief 标量除法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前矩阵
     */
    Matrix2x2<T>& operator/=(T scalar)
    {
        mRows[0] /= scalar;
        mRows[1] /= scalar;
        return *this;
    }

    /**
     * @brief 转置矩阵
     * @return 转置后的矩阵
     */
    Matrix2x2<T> Transpose() const
    {
        Matrix2x2<T> result;
        result.mRows[0].x = mRows[0].x;
        result.mRows[0].y = mRows[1].x;
        result.mRows[1].x = mRows[0].y;
        result.mRows[1].y = mRows[1].y;
        return result;
    }

    /**
     * @brief 计算矩阵行列式
     * @return 行列式值
     */
    T Determinant() const
    {
        return mRows[0].x * mRows[1].y - mRows[0].y * mRows[1].x;
    }

    /**
     * @brief 求逆矩阵
     * @return 逆矩阵
     */
    Matrix2x2<T> Inverse() const
    {
        T det = Determinant();
        if (det == T(0))
        {
            return Matrix2x2<T>();
        }
        T invDet = T(1) / det;
        Matrix2x2<T> result;
        result.mRows[0].x = mRows[1].y * invDet;
        result.mRows[0].y = -mRows[0].y * invDet;
        result.mRows[1].x = -mRows[1].x * invDet;
        result.mRows[1].y = mRows[0].x * invDet;
        return result;
    }

    /**
     * @brief 创建单位矩阵
     * @return 单位矩阵
     */
    static Matrix2x2<T> Identity()
    {
        return Matrix2x2<T>(
            T(1), T(0),
            T(0), T(1)
        );
    }

    /**
     * @brief 创建零矩阵
     * @return 零矩阵
     */
    static Matrix2x2<T> Zero()
    {
        return Matrix2x2<T>(
            T(0), T(0),
            T(0), T(0)
        );
    }
};

/**
 * @brief 3x3矩阵模板类
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct Matrix3x3
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mRows[3]; ///< 矩阵的行向量

public:
    /**
     * @brief 默认构造函数
     */
    Matrix3x3() = default;

    /**
     * @brief 带参数的构造函数
     * @param m00 第一行第一列元素
     * @param m01 第一行第二列元素
     * @param m02 第一行第三列元素
     * @param m10 第二行第一列元素
     * @param m11 第二行第二列元素
     * @param m12 第二行第三列元素
     * @param m20 第三行第一列元素
     * @param m21 第三行第二列元素
     * @param m22 第三行第三列元素
     */
    Matrix3x3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22)
    {
        mRows[0] = TVector3<T>(m00, m01, m02);
        mRows[1] = TVector3<T>(m10, m11, m12);
        mRows[2] = TVector3<T>(m20, m21, m22);
    }

    /**
     * @brief 矩阵加法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的和
     */
    Matrix3x3<T> operator+(const Matrix3x3<T>& mat) const
    {
        Matrix3x3<T> result;
        result.mRows[0] = mRows[0] + mat.mRows[0];
        result.mRows[1] = mRows[1] + mat.mRows[1];
        result.mRows[2] = mRows[2] + mat.mRows[2];
        return result;
    }

    /**
     * @brief 矩阵减法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的差
     */
    Matrix3x3<T> operator-(const Matrix3x3<T>& mat) const
    {
        Matrix3x3<T> result;
        result.mRows[0] = mRows[0] - mat.mRows[0];
        result.mRows[1] = mRows[1] - mat.mRows[1];
        result.mRows[2] = mRows[2] - mat.mRows[2];
        return result;
    }

    /**
     * @brief 矩阵乘法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的乘积
     */
    Matrix3x3<T> operator*(const Matrix3x3<T>& mat) const
    {
        Matrix3x3<T> result;
        result.mRows[0].x = mRows[0].x * mat.mRows[0].x + mRows[0].y * mat.mRows[1].x + mRows[0].z * mat.mRows[2].x;
        result.mRows[0].y = mRows[0].x * mat.mRows[0].y + mRows[0].y * mat.mRows[1].y + mRows[0].z * mat.mRows[2].y;
        result.mRows[0].z = mRows[0].x * mat.mRows[0].z + mRows[0].y * mat.mRows[1].z + mRows[0].z * mat.mRows[2].z;
        result.mRows[1].x = mRows[1].x * mat.mRows[0].x + mRows[1].y * mat.mRows[1].x + mRows[1].z * mat.mRows[2].x;
        result.mRows[1].y = mRows[1].x * mat.mRows[0].y + mRows[1].y * mat.mRows[1].y + mRows[1].z * mat.mRows[2].y;
        result.mRows[1].z = mRows[1].x * mat.mRows[0].z + mRows[1].y * mat.mRows[1].z + mRows[1].z * mat.mRows[2].z;
        result.mRows[2].x = mRows[2].x * mat.mRows[0].x + mRows[2].y * mat.mRows[1].x + mRows[2].z * mat.mRows[2].x;
        result.mRows[2].y = mRows[2].x * mat.mRows[0].y + mRows[2].y * mat.mRows[1].y + mRows[2].z * mat.mRows[2].y;
        result.mRows[2].z = mRows[2].x * mat.mRows[0].z + mRows[2].y * mat.mRows[1].z + mRows[2].z * mat.mRows[2].z;
        return result;
    }

    /**
     * @brief 矩阵与向量乘法
     * @param vec 向量
     * @return 乘积结果向量
     */
    TVector3<T> operator*(const TVector3<T>& vec) const
    {
        TVector3<T> result;
        result.x = mRows[0].x * vec.x + mRows[0].y * vec.y + mRows[0].z * vec.z;
        result.y = mRows[1].x * vec.x + mRows[1].y * vec.y + mRows[1].z * vec.z;
        result.z = mRows[2].x * vec.x + mRows[2].y * vec.y + mRows[2].z * vec.z;
        return result;
    }

    /**
     * @brief 标量乘法运算符
     * @param scalar 标量值
     * @return 矩阵与标量的乘积
     */
    Matrix3x3<T> operator*(T scalar) const
    {
        Matrix3x3<T> result;
        result.mRows[0] = mRows[0] * scalar;
        result.mRows[1] = mRows[1] * scalar;
        result.mRows[2] = mRows[2] * scalar;
        return result;
    }

    /**
     * @brief 标量除法运算符
     * @param scalar 标量值
     * @return 矩阵与标量的商
     */
    Matrix3x3<T> operator/(T scalar) const
    {
        Matrix3x3<T> result;
        result.mRows[0] = mRows[0] / scalar;
        result.mRows[1] = mRows[1] / scalar;
        result.mRows[2] = mRows[2] / scalar;
        return result;
    }

    /**
     * @brief 矩阵加法复合赋值运算符
     * @param mat 另一个矩阵
     * @return 引用到当前矩阵
     */
    Matrix3x3<T>& operator+=(const Matrix3x3<T>& mat)
    {
        mRows[0] += mat.mRows[0];
        mRows[1] += mat.mRows[1];
        mRows[2] += mat.mRows[2];
        return *this;
    }

    /**
     * @brief 矩阵减法复合赋值运算符
     * @param mat 另一个矩阵
     * @return 引用到当前矩阵
     */
    Matrix3x3<T>& operator-=(const Matrix3x3<T>& mat)
    {
        mRows[0] -= mat.mRows[0];
        mRows[1] -= mat.mRows[1];
        mRows[2] -= mat.mRows[2];
        return *this;
    }

    /**
     * @brief 标量乘法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前矩阵
     */
    Matrix3x3<T>& operator*=(T scalar)
    {
        mRows[0] *= scalar;
        mRows[1] *= scalar;
        mRows[2] *= scalar;
        return *this;
    }

    /**
     * @brief 标量除法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前矩阵
     */
    Matrix3x3<T>& operator/=(T scalar)
    {
        mRows[0] /= scalar;
        mRows[1] /= scalar;
        mRows[2] /= scalar;
        return *this;
    }

    /**
     * @brief 转置矩阵
     * @return 转置后的矩阵
     */
    Matrix3x3<T> Transpose() const
    {
        Matrix3x3<T> result;
        result.mRows[0].x = mRows[0].x;
        result.mRows[0].y = mRows[1].x;
        result.mRows[0].z = mRows[2].x;
        result.mRows[1].x = mRows[0].y;
        result.mRows[1].y = mRows[1].y;
        result.mRows[1].z = mRows[2].y;
        result.mRows[2].x = mRows[0].z;
        result.mRows[2].y = mRows[1].z;
        result.mRows[2].z = mRows[2].z;
        return result;
    }

    /**
     * @brief 计算矩阵行列式
     * @return 行列式值
     */
    T Determinant() const
    {
        return mRows[0].x * (mRows[1].y * mRows[2].z - mRows[1].z * mRows[2].y) -
               mRows[0].y * (mRows[1].x * mRows[2].z - mRows[1].z * mRows[2].x) +
               mRows[0].z * (mRows[1].x * mRows[2].y - mRows[1].y * mRows[2].x);
    }

    /**
     * @brief 创建单位矩阵
     * @return 单位矩阵
     */
    static Matrix3x3<T> Identity()
    {
        return Matrix3x3<T>(
            T(1), T(0), T(0),
            T(0), T(1), T(0),
            T(0), T(0), T(1)
        );
    }

    /**
     * @brief 创建零矩阵
     * @return 零矩阵
     */
    static Matrix3x3<T> Zero()
    {
        return Matrix3x3<T>(
            T(0), T(0), T(0),
            T(0), T(0), T(0),
            T(0), T(0), T(0)
        );
    }
};

/**
 * @brief 4x4矩阵模板类
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct Matrix4x4
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector4<T> mRows[4]; ///< 矩阵的行向量

public:
    /**
     * @brief 默认构造函数
     */
    Matrix4x4() = default;

    /**
     * @brief 带参数的构造函数
     * @param m00 第一行第一列元素
     * @param m01 第一行第二列元素
     * @param m02 第一行第三列元素
     * @param m03 第一行第四列元素
     * @param m10 第二行第一列元素
     * @param m11 第二行第二列元素
     * @param m12 第二行第三列元素
     * @param m13 第二行第四列元素
     * @param m20 第三行第一列元素
     * @param m21 第三行第二列元素
     * @param m22 第三行第三列元素
     * @param m23 第三行第四列元素
     * @param m30 第四行第一列元素
     * @param m31 第四行第二列元素
     * @param m32 第四行第三列元素
     * @param m33 第四行第四列元素
     */
    Matrix4x4(
        T m00, T m01, T m02, T m03,
        T m10, T m11, T m12, T m13,
        T m20, T m21, T m22, T m23,
        T m30, T m31, T m32, T m33
    )
    {
        mRows[0] = TVector4<T>(m00, m01, m02, m03);
        mRows[1] = TVector4<T>(m10, m11, m12, m13);
        mRows[2] = TVector4<T>(m20, m21, m22, m23);
        mRows[3] = TVector4<T>(m30, m31, m32, m33);
    }

    /**
     * @brief 矩阵加法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的和
     */
    Matrix4x4<T> operator+(const Matrix4x4<T>& mat) const
    {
        Matrix4x4<T> result;
        result.mRows[0] = mRows[0] + mat.mRows[0];
        result.mRows[1] = mRows[1] + mat.mRows[1];
        result.mRows[2] = mRows[2] + mat.mRows[2];
        result.mRows[3] = mRows[3] + mat.mRows[3];
        return result;
    }

    /**
     * @brief 矩阵减法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的差
     */
    Matrix4x4<T> operator-(const Matrix4x4<T>& mat) const
    {
        Matrix4x4<T> result;
        result.mRows[0] = mRows[0] - mat.mRows[0];
        result.mRows[1] = mRows[1] - mat.mRows[1];
        result.mRows[2] = mRows[2] - mat.mRows[2];
        result.mRows[3] = mRows[3] - mat.mRows[3];
        return result;
    }

    /**
     * @brief 矩阵乘法运算符
     * @param mat 另一个矩阵
     * @return 两个矩阵的乘积
     */
    Matrix4x4<T> operator*(const Matrix4x4<T>& mat) const
    {
        Matrix4x4<T> result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.mRows[i][j] = 
                    mRows[i][0] * mat.mRows[0][j] +
                    mRows[i][1] * mat.mRows[1][j] +
                    mRows[i][2] * mat.mRows[2][j] +
                    mRows[i][3] * mat.mRows[3][j];
            }
        }
        return result;
    }

    /**
     * @brief 矩阵与向量乘法
     * @param vec 向量
     * @return 乘积结果向量
     */
    TVector4<T> operator*(const TVector4<T>& vec) const
    {
        TVector4<T> result;
        result.x = mRows[0].x * vec.x + mRows[0].y * vec.y + mRows[0].z * vec.z + mRows[0].w * vec.w;
        result.y = mRows[1].x * vec.x + mRows[1].y * vec.y + mRows[1].z * vec.z + mRows[1].w * vec.w;
        result.z = mRows[2].x * vec.x + mRows[2].y * vec.y + mRows[2].z * vec.z + mRows[2].w * vec.w;
        result.w = mRows[3].x * vec.x + mRows[3].y * vec.y + mRows[3].z * vec.z + mRows[3].w * vec.w;
        return result;
    }

    /**
     * @brief 标量乘法运算符
     * @param scalar 标量值
     * @return 矩阵与标量的乘积
     */
    Matrix4x4<T> operator*(T scalar) const
    {
        Matrix4x4<T> result;
        result.mRows[0] = mRows[0] * scalar;
        result.mRows[1] = mRows[1] * scalar;
        result.mRows[2] = mRows[2] * scalar;
        result.mRows[3] = mRows[3] * scalar;
        return result;
    }

    /**
     * @brief 标量除法运算符
     * @param scalar 标量值
     * @return 矩阵与标量的商
     */
    Matrix4x4<T> operator/(T scalar) const
    {
        Matrix4x4<T> result;
        result.mRows[0] = mRows[0] / scalar;
        result.mRows[1] = mRows[1] / scalar;
        result.mRows[2] = mRows[2] / scalar;
        result.mRows[3] = mRows[3] / scalar;
        return result;
    }

    /**
     * @brief 矩阵加法复合赋值运算符
     * @param mat 另一个矩阵
     * @return 引用到当前矩阵
     */
    Matrix4x4<T>& operator+=(const Matrix4x4<T>& mat)
    {
        mRows[0] += mat.mRows[0];
        mRows[1] += mat.mRows[1];
        mRows[2] += mat.mRows[2];
        mRows[3] += mat.mRows[3];
        return *this;
    }

    /**
     * @brief 矩阵减法复合赋值运算符
     * @param mat 另一个矩阵
     * @return 引用到当前矩阵
     */
    Matrix4x4<T>& operator-=(const Matrix4x4<T>& mat)
    {
        mRows[0] -= mat.mRows[0];
        mRows[1] -= mat.mRows[1];
        mRows[2] -= mat.mRows[2];
        mRows[3] -= mat.mRows[3];
        return *this;
    }

    /**
     * @brief 标量乘法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前矩阵
     */
    Matrix4x4<T>& operator*=(T scalar)
    {
        mRows[0] *= scalar;
        mRows[1] *= scalar;
        mRows[2] *= scalar;
        mRows[3] *= scalar;
        return *this;
    }

    /**
     * @brief 标量除法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前矩阵
     */
    Matrix4x4<T>& operator/=(T scalar)
    {
        mRows[0] /= scalar;
        mRows[1] /= scalar;
        mRows[2] /= scalar;
        mRows[3] /= scalar;
        return *this;
    }

    /**
     * @brief 转置矩阵
     * @return 转置后的矩阵
     */
    Matrix4x4<T> Transpose() const
    {
        Matrix4x4<T> result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.mRows[i][j] = mRows[j][i];
            }
        }
        return result;
    }

    /**
     * @brief 创建单位矩阵
     * @return 单位矩阵
     */
    static Matrix4x4<T> Identity()
    {
        return Matrix4x4<T>(
            T(1), T(0), T(0), T(0),
            T(0), T(1), T(0), T(0),
            T(0), T(0), T(1), T(0),
            T(0), T(0), T(0), T(1)
        );
    }

    /**
     * @brief 创建零矩阵
     * @return 零矩阵
     */
    static Matrix4x4<T> Zero()
    {
        return Matrix4x4<T>(
            T(0), T(0), T(0), T(0),
            T(0), T(0), T(0), T(0),
            T(0), T(0), T(0), T(0),
            T(0), T(0), T(0), T(0)
        );
    }

    /**
     * @brief 下标访问运算符
     * @param index 行索引
     * @return 对应行的向量引用
     */
    TVector4<T>& operator[](int index)
    {
        return mRows[index];
    }

    /**
     * @brief 常量下标访问运算符
     * @param index 行索引
     * @return 对应行的常量向量引用
     */
    const TVector4<T>& operator[](int index) const
    {
        return mRows[index];
    }
};

} // namespace SympConv

#endif // SYMPCONV_MATRIX_H