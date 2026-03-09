#ifndef SYMPCONV_VECTOR_H
#define SYMPCONV_VECTOR_H

#include <type_traits>
#include <cmath>

namespace SympConv {

	template<typename T>
	struct TVector2
	{
		static_assert(std::is_floating_point_v<T>, "T must be floating point");

	public:
		T x;
		T y;

	public:
		TVector2() = default;
		TVector2(T InX, T InY)
			: x(InX)
			, y(InY)
		{

		}

		TVector2<T> operator+(const TVector2<T>& V) const
		{
			return TVector2<T>(x + V.x, y + V.y);
		}

		TVector2<T> operator-(const TVector2<T>& V) const
		{
			return TVector2<T>(x - V.x, y - V.y);
		}

		T Dot(const TVector2<T>& V) const
		{
			return x * V.x + y * V.y;
		}

		static T DotProduct(const TVector2<T>& A, const TVector2<T>& B)
		{
			return A.x * B.x + A.y * B.y;
		}

		// 标量乘法
		TVector2<T> operator*(T scalar) const
		{
			return TVector2<T>(x * scalar, y * scalar);
		}

		// 标量除法
		TVector2<T> operator/(T scalar) const
		{
			return TVector2<T>(x / scalar, y / scalar);
		}

		// 复合赋值运算符
		TVector2<T>& operator+=(const TVector2<T>& V)
		{
			x += V.x;
			y += V.y;
			return *this;
		}

		TVector2<T>& operator-=(const TVector2<T>& V)
		{
			x -= V.x;
			y -= V.y;
			return *this;
		}

		TVector2<T>& operator*=(T scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		TVector2<T>& operator/=(T scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		// 长度计算
		T Length() const
		{
			return std::sqrt(x * x + y * y);
		}

		T LengthSquared() const
		{
			return x * x + y * y;
		}

		// 归一化
		TVector2<T> Normalize() const
		{
			T len = Length();
			if (len > T(0))
			{
				return *this / len;
			}
			return TVector2<T>(T(0), T(0));
		}

		// 线性插值
		static TVector2<T> Lerp(const TVector2<T>& A, const TVector2<T>& B, T t)
		{
			return TVector2<T>(
				A.x + (B.x - A.x) * t,
				A.y + (B.y - A.y) * t
			);
		}
	};

	template<typename T>
	struct TVector3
	{
		static_assert(std::is_floating_point_v<T>, "T must be floating point");

	public:
		T x;
		T y;
		T z;

	public:
		TVector3() = default;
		TVector3(T InX, T InY, T InZ)
			:x(InX)
			,y(InY)
			,z(InZ)
		{

		}

		TVector3<T> operator+(const TVector3<T>& V) const
		{
			return TVector3<T>(x + V.x, y + V.y, z + V.z);
		}

		TVector3<T> operator-(const TVector3<T>& V) const
		{
			return TVector3<T>(x - V.x, y - V.y, z - V.z);
		}

		T Dot(const TVector3<T>& V) const
		{
			return x * V.x + y * V.y + z * V.z;
		}

		static T DotProduct(const TVector3<T>& A, const TVector3<T>& B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z;
		}

		TVector3<T> Cross(const TVector3<T>& V) const
		{
			return TVector3<T>(
				y * V.z - z * V.y,
				z * V.x - x * V.z,
				x * V.y - y * V.x
			);
		}

		static TVector3<T> CrossProduct(const TVector3<T>& A, const TVector3<T>& B)
		{
			return TVector3<T>(
				A.y * B.z - A.z * B.y,
				A.z * B.x - A.x * B.z,
				A.x * B.y - A.y * B.x
			);
		}

		// 标量乘法
		TVector3<T> operator*(T scalar) const
		{
			return TVector3<T>(x * scalar, y * scalar, z * scalar);
		}

		// 标量除法
		TVector3<T> operator/(T scalar) const
		{
			return TVector3<T>(x / scalar, y / scalar, z / scalar);
		}

		// 复合赋值运算符
		TVector3<T>& operator+=(const TVector3<T>& V)
		{
			x += V.x;
			y += V.y;
			z += V.z;
			return *this;
		}

		TVector3<T>& operator-=(const TVector3<T>& V)
		{
			x -= V.x;
			y -= V.y;
			z -= V.z;
			return *this;
		}

		TVector3<T>& operator*=(T scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		TVector3<T>& operator/=(T scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		// 长度计算
		T Length() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		T LengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		// 归一化
		TVector3<T> Normalize() const
		{
			T len = Length();
			if (len > T(0))
			{
				return *this / len;
			}
			return TVector3<T>(T(0), T(0), T(0));
		}

		// 线性插值
		static TVector3<T> Lerp(const TVector3<T>& A, const TVector3<T>& B, T t)
		{
			return TVector3<T>(
				A.x + (B.x - A.x) * t,
				A.y + (B.y - A.y) * t,
				A.z + (B.z - A.z) * t
			);
		}
	};

	template<typename T>
	struct TVector4
	{
		static_assert(std::is_floating_point_v<T>, "T must be floating point");

	public:
		T x;
		T y;
		T z;
		T w;

	public:
		TVector4() = default;
		TVector4(T InX, T InY, T InZ, T InW)
			: x(InX)
			, y(InY)
			, z(InZ)
			, w(InW)
		{

		}

		// 向量加法
		TVector4<T> operator+(const TVector4<T>& V) const
		{
			return TVector4<T>(x + V.x, y + V.y, z + V.z, w + V.w);
		}

		// 向量减法
		TVector4<T> operator-(const TVector4<T>& V) const
		{
			return TVector4<T>(x - V.x, y - V.y, z - V.z, w - V.w);
		}

		// 标量乘法
		TVector4<T> operator*(T scalar) const
		{
			return TVector4<T>(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		// 标量除法
		TVector4<T> operator/(T scalar) const
		{
			return TVector4<T>(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		// 复合赋值运算符
		TVector4<T>& operator+=(const TVector4<T>& V)
		{
			x += V.x;
			y += V.y;
			z += V.z;
			w += V.w;
			return *this;
		}

		TVector4<T>& operator-=(const TVector4<T>& V)
		{
			x -= V.x;
			y -= V.y;
			z -= V.z;
			w -= V.w;
			return *this;
		}

		TVector4<T>& operator*=(T scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		TVector4<T>& operator/=(T scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}

		// 点积
		T Dot(const TVector4<T>& V) const
		{
			return x * V.x + y * V.y + z * V.z + w * V.w;
		}

		static T DotProduct(const TVector4<T>& A, const TVector4<T>& B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
		}

		// 长度计算
		T Length() const
		{
			return std::sqrt(x * x + y * y + z * z + w * w);
		}

		T LengthSquared() const
		{
			return x * x + y * y + z * z + w * w;
		}

		// 归一化
		TVector4<T> Normalize() const
		{
			T len = Length();
			if (len > T(0))
			{
				return *this / len;
			}
			return TVector4<T>(T(0), T(0), T(0), T(0));
		}

		// 线性插值
		static TVector4<T> Lerp(const TVector4<T>& A, const TVector4<T>& B, T t)
		{
			return TVector4<T>(
				A.x + (B.x - A.x) * t,
				A.y + (B.y - A.y) * t,
				A.z + (B.z - A.z) * t,
				A.w + (B.w - A.w) * t
			);
		}
	};
}
#endif // SYMPCONV_VECTOR_H
