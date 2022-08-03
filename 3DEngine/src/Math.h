#pragma once

#include <cmath>
#include <DirectXMath.h>

namespace Math
{
	constexpr float PI = 3.1415926535897932384626433832795f;
	constexpr float _2PI = 2.0f * PI;

	// Radians to degrees
	constexpr float Degrees(const float radians)
	{
		return radians * (180.0f / PI);
	}

	// Degrees to radians
	constexpr float Radians(const float degrees)
	{
		return degrees * (PI / 180.0f);
	}

	template<typename T>
	inline T Deadzone(T val, T deadzone)
	{
		if (std::abs(val) < deadzone)
		{
			return T(0);
		}

		return val;
	}

	template <typename T>
	constexpr auto sqPow(const T& x)
	{
		return x * x;
	}

	// Normalize a value in the range [min, max]
	template<typename T, typename U>
	inline T NormalizeRange(U x, U min, U max)
	{
		return T(x - min) / T(max - min);
	}

	// Shift and bias a value into another range
	template<typename T, typename U>
	inline T ShiftBias(U x, U shift, U bias)
	{
		return T(x * bias) + T(shift);
	}

	template<typename T>
	inline T AlignUpWithMask(T value, size_t mask)
	{
		return (T)(((size_t)value + mask) & ~mask);
	}

	template <typename T>
	inline T AlignDownWithMask(T value, size_t mask)
	{
		return (T)((size_t)value & ~mask);
	}

	template <typename T>
	inline T AlignUp(T value, size_t alignment)
	{
		return AlignUpWithMask(value, alignment - 1);
	}

	template <typename T>
	inline T AlignDown(T value, size_t alignment)
	{
		return AlignDownWithMask(value, alignment - 1);
	}

	template <typename T>
	inline bool IsAligned(T value, size_t alignment)
	{
		return 0 == ((size_t)value & (alignment - 1));
	}

	template <typename T>
	inline T DivideByMultiple(T value, size_t alignment)
	{
		return (T)((value + alignment - 1) / alignment);
	}

	/**
   * Round up to the next highest power of 2.
   * @source: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
   */
	inline uint32_t NextHighestPow2(uint32_t v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;

		return v;
	}

	/**
	* Round up to the next highest power of 2.
	* @source: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	*/
	inline uint64_t NextHighestPow2(uint64_t v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v |= v >> 32;
		v++;

		return v;
	}

	template<class T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<class T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<class T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	template<class T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	template<class T>
	static T WrapAngle(T theta)
	{
		const T modded = fmod(theta, (T)2.0 * (T)PI);
		return (modded > (T)PI) ?
			(modded - (T)2.0 * (T)PI) :
			modded;
	}

	//Polar angle of the point (x, y) in [0, 2*PI]
	static float AngleFromXY(float x, float y)
	{
		float theta = 0.0f;

		//Quadrant I or IV
		if (x >= 0.0f)
		{
			theta = atanf(y / x);

			//If x = 0, then atanf(y/x) = +pi/2
			//		if y > 0 atanf(y/x) = -pi/2
			//		if y < 0 in [-pi/2, +pi/2]
			if (theta < 0.0f)
			{
				theta += 2.0f * PI; // in [0, 2*pi]
			}
		}

		//Quadrant II or III
		else
		{
			theta = atanf(y / x) + PI; //in [0, 2*pi]
		}

		return theta;
	}

	static DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi)
	{
		return DirectX::XMVectorSet(
			radius * sinf(phi) * cosf(theta),
			radius * cosf(phi),
			radius * sinf(phi) * sinf(theta),
			1.0f);
	}

	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
	{
		//The inverse transpose is just applied to normals. So zero
		//out translation row so that it doesn't get into out inverse-transpose
		//calculation. We do not want the inverse-transpose of the translation
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
		return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
	}

	static DirectX::XMFLOAT4X4 Identity4x4()
	{
		static DirectX::XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return I;
	}

	static const float infinity;
}