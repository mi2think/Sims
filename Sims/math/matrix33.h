/********************************************************************
	created:	2016/08/08
	created:	8:8:2016   11:22
	filename: 	D:\Code\Sims\Sims\math\matrix33.h
	file path:	D:\Code\Sims\Sims\math
	file base:	matrix33
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Matrix33
*********************************************************************/
#ifndef __MATRIX33_H__
#define __MATRIX33_H__

#include "math_fwd.h"
#include "vector3.h"

namespace sims
{
	template <typename T>
	class Matrix33
	{
	public:

		enum { R = 3, C = 3, N = R * C };
		union
		{
			struct
			{
				T m11, m12, m13;
				T m21, m22, m23;
				T m31, m32, m33;
			};
			T m[R][C];
		};

		Matrix33() {}
		Matrix33(const T* _m) { memcpy(m, _m, sizeof(T) * N); }
		Matrix33(const Matrix33& _m) { memcpy(m, &_m, sizeof(T) * N); }
		Matrix33(T _m11, T _m12, T _m13,
			T _m21, T _m22, T _m23,
			T _m31, T _m32, T _m33)
			: m11(_m11), m12(_m12), m13(_m13)
			, m21(_m21), m22(_m22), m23(_m23)
			, m31(_m31), m32(_m32), m33(_m33) {}

		bool operator==(const Matrix33& _m) const
		{
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					if (!equal_t(m[i][j], _m.m[i][j]))
						return false;
				}
			}
			return true;
		}
		bool operator!=(const Matrix33& _m) const { return !operator==(_m); }

		Matrix33& operator+=(const Matrix33& _m)
		{
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					m[i][j] += _m.m[i][j];
				}
			}
			return *this;
		}

		Matrix33 operator+(const Matrix33& _m) const
		{
			Matrix33 n = *this;
			n += _m;
			return n;
		}

		Matrix33& operator-=(const Matrix33& _m)
		{
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					m[i][j] -= _m.m[i][j];
				}
			}
			return *this;
		}

		Matrix33 operator-(const Matrix33& _m) const
		{
			Matrix33 n = *this;
			n -= _m;
			return n;
		}

		template <typename U>
		Matrix33& operator*=(U k)
		{
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					m[i][j] *= k;
				}
			}
			return *this;
		}
		template <typename U>
		Matrix33 operator*(U k) const
		{
			Matrix33 n = *this;
			n *= k;
			return n;
		}

		template <typename U>
		Matrix33& operator/=(U k)
		{
			float f = 1.0f / k;
			return operator*=(f);
		}
		template <typename U>
		Matrix33 operator/(U k) const
		{
			Matrix33 n = *this;
			n /= k;
			return n;
		}

		Matrix33 operator-() const
		{
			Matrix33 n;
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					n.m[i][j] = -m[i][j];
				}
			}
			return n;
		}

		Matrix33 operator*(const Matrix33& _m) const
		{
			Matrix33 n;
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					T sum{ 0 };
					for (int k = 0; k < C; ++k)
					{
						sum += m[i][k] * _m.m[k][j];
					}
					n.m[i][j] = sum;
				}
			}
			return n;
		}

		Matrix33& operator *=(const Matrix33& _m)
		{
			*this = *this * _m;
			return *this;
		}

		Matrix33& Identity()
		{
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					i == j ? m[i][j] = 1 : m[i][j] = 0;
				}
			}
			return *this;
		}

		bool IsIdentity() const
		{
			for (int i = 0; i < R; ++i)
			{
				for (int j = 0; j < C; ++j)
				{
					if (i == j)
					{
						if (!equal_t(m[i][j], T(1)))
							return false;
					}
					else
					{
						if (!equal_t(m[i][j], T(0)))
							return false;
					}
				}
			}
			return true;
		}

		// Transpose
		Matrix33& Transpose()
		{
			for (int i = 0; i < R; ++i)
			{
				for (int j = i; j < C; ++j)
				{
					if (i != j)
					{
						swap_t(m[i][j], m[j][i]);
					}
				}
			}
			return *this;
		}

		string ToString() const
		{
			ostringstream oss;
			for (int i = 0; i < R; ++i)
			{
				oss << "\n[" << m[i][0] << " " << m[i][1] << " " << m[i][2] << "]";
			}
			return oss.str();
		}
	};

	template <typename T, typename U>
	inline Matrix33<T> operator*(U k, const Matrix33<T>& _m)
	{
		Matrix33<T> n = _m;
		n *= k;
		return n;
	}

	template <typename T>
	inline Matrix33<T>& MatrixMultiply(Matrix33<T>& n, const Matrix33<T>& m1, const Matrix33<T>& m2)
	{
		for (int i = 0; i < Matrix33<T>::R; ++i)
		{
			for (int j = 0; j < Matrix33<T>::C; ++j)
			{
				T sum{ 0 };
				for (int k = 0; k < Matrix33<T>::C; ++k)
				{
					sum += m1.m[i][k] * m2.m[k][j];
				}
				n.m[i][j] = sum;
			}
		}
		return n;
	}

	// Matrix n and _m must not be same one
	template <typename T>
	inline Matrix33<T>& MatrixTranspose(Matrix33<T>& n, const Matrix33<T>& _m)
	{
		assert(&n != &_m);

		n.m11 = _m.m11; n.m12 = _m.m21; n.m13 = _m.m31;
		n.m21 = _m.m12; n.m22 = _m.m22; n.m23 = _m.m32;
		n.m31 = _m.m13; n.m32 = _m.m23; n.m33 = _m.m33;
		return n;
	}

	template <typename T>
	inline float MatrixDeterminant(const Matrix33<T>& _m)
	{
		return _m.m11 * (_m.m22 * _m.m33 - _m.m23 * _m.m32) + _m.m12 * (_m.m23 * _m.m31 - _m.m21 * _m.m33)
			+ _m.m13 * (_m.m21 * _m.m32 - _m.m22 * _m.m31);
	}

	// algebraic cofactor
	template <typename T>
	inline Matrix33<T>& MatrixAlgebraicCofactor(Matrix33<T>& n, const Matrix33<T>& _m)
	{
		n.m11 = +(_m.m22 * _m.m33 - _m.m32 * _m.m23);
		n.m12 = -(_m.m21 * _m.m33 - _m.m31 * _m.m23);
		n.m13 = +(_m.m21 * _m.m32 - _m.m31 * _m.m22);

		n.m21 = -(_m.m12 * _m.m33 - _m.m32 * _m.m13);
		n.m22 = +(_m.m11 * _m.m33 - _m.m31 * _m.m13);
		n.m23 = -(_m.m11 * _m.m32 - _m.m31 * _m.m12);

		n.m31 = +(_m.m12 * _m.m23 - _m.m22 * _m.m13);
		n.m32 = -(_m.m11 * _m.m23 - _m.m21 * _m.m13);
		n.m33 = +(_m.m11 * _m.m22 - _m.m21 * _m.m12);
		return n;
	}

	// standard adjugate
	template <typename T>
	inline Matrix33<T>& MatrixStandardAdjugate(Matrix33<T>& n, const Matrix33<T>& _m)
	{
		Matrix33<T> t;
		MatrixAlgebraicCofactor(t, _m);
		MatrixTranspose(n, t);
		return n;
	}

	template <typename T>
	inline Matrix33<T>& MatrixInverse(Matrix33<T>& n, const Matrix33<T>& _m)
	{
		float det = MatrixDeterminant(_m);

		assert(ABS(det) > EPSILON_E5);

		MatrixStandardAdjugate(n, _m);
		n /= det;
		return n;
	}


	// Scale

	// scale by coordinate axis
	template <typename T>
	inline Matrix33<T>& MatrixScaling(Matrix33<T>& n, const Vector3<T>& v)
	{
		n.m11 = v.x;  n.m12 = 0.0f; n.m13 = 0.0f;
		n.m21 = 0.0f; n.m22 = v.y;  n.m23 = 0.0f;
		n.m31 = 0.0f; n.m32 = 0.0f; n.m33 = v.z;
		return n;
	}

	// Rotate

	template <typename T>
	inline Matrix33<T>& MatrixRotationX(Matrix33<T>& n, float radian)
	{
		float s = 0.0f;
		float c = 0.0f;
		sincos(s, c, radian);

		n.m11 = 1.0f; n.m12 = 0.0f; n.m13 = 0.0f;
		n.m21 = 0.0f; n.m22 = c;	n.m23 = s;
		n.m31 = 0.0f; n.m32 = -s;	n.m33 = c;
		return n;
	}

	template <typename T>
	inline Matrix33<T>& MatrixRotationY(Matrix33<T>& n, float radian)
	{
		float s = 0.0f;
		float c = 0.0f;
		sincos(s, c, radian);

		n.m11 = c;    n.m12 = 0.0f; n.m13 = -s;
		n.m21 = 0.0f; n.m22 = 1.0f;	n.m23 = 0;
		n.m31 = s;	  n.m32 = 0.0f;	n.m33 = c;
		return n;
	}

	template <typename T>
	inline Matrix33<T>& MatrixRotationZ(Matrix33<T>& n, float radian)
	{
		float s = 0.0f;
		float c = 0.0f;
		sincos(s, c, radian);

		n.m11 = c;    n.m12 = s;    n.m13 = 0.0f;
		n.m21 = -s;   n.m22 = c;	n.m23 = 0.0f;
		n.m31 = 0.0f; n.m32 = 0.0f;	n.m33 = 1.0f;
		return n;
	}

	template <typename T>
	inline Matrix33<T>& MatrixRotationXYZ(Matrix33<T>& m, float radianX, float radianY, float radianZ)
	{
		Matrix33<T> mx;
		MatrixRotationX(mx, radianX);

		Matrix33<T> my;
		MatrixRotationY(my, radianY);

		Matrix33<T> mz;
		MatrixRotationZ(mz, radianZ);

		Matrix33<T> n;
		MatrixMultiply(n, mx, my);

		return MatrixMultiply(m, n, mz);
	}

	template <typename T>
	inline Matrix33<T>& MatrixRotationZYX(Matrix33<T>& m, float radianX, float radianY, float radianZ)
	{
		Matrix33<T> mx;
		MatrixRotationX(mx, radianX);

		Matrix33<T> my;
		MatrixRotationY(my, radianY);

		Matrix33<T> mz;
		MatrixRotationZ(mz, radianZ);

		Matrix33<T> n;
		MatrixMultiply(n, mz, my);

		return MatrixMultiply(m, n, mx);
	}

	// Assume v through the origin, v must be normal Vector
	template <typename T>
	inline Matrix33<T>& MatrixRotationAxis(Matrix33<T>& n, const Vector3<T>& v, float radian)
	{
		assert(v.IsNormalized());

		float s = 0.0f;
		float c = 0.0f;
		sincos(s, c, radian);

		// Common expr 
		float a = 1.0f - c;
		float ax = a * v.x;
		float ay = a * v.y;
		float az = a * v.z;

		n.m11 = ax * v.x + c;
		n.m12 = ax * v.y + v.z * s;
		n.m13 = ax * v.z - v.y * s;

		n.m21 = ay * v.x - v.z * s;
		n.m22 = ay * v.y + c;
		n.m23 = ay * v.z + v.x * s;

		n.m31 = az * v.x + v.y * s;
		n.m32 = az * v.y - v.x * s;
		n.m33 = az * v.z + c;

		return n;
	}

	//////////////////////////////////////////////////////////////////////////

	template <typename T, typename U>
	inline Vector3<T> operator*(const Vector3<T>& _v, const Matrix33<U>& _m)
	{
		Vector3<T> v;
		v.x = T(_v.x * _m.m11 + _v.y * _m.m21 + _v.z * _m.m31);
		v.y = T(_v.x * _m.m12 + _v.y * _m.m22 + _v.z * _m.m32);
		v.z = T(_v.x * _m.m13 + _v.y * _m.m23 + _v.z * _m.m33);
		return v;
	}

	template <typename T, typename U>
	inline Vector3<T>& operator*=(Vector3<T>& v, const Matrix33<U>& _m)
	{
		v = v * _m;
		return v;
	}
}

#endif
