/********************************************************************
	created:	2016/09/05
	created:	5:9:2016   17:30
	filename: 	D:\Code\Sims\Sims\math\quat.h
	file path:	D:\Code\Sims\Sims\math
	file base:	quat
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Quat
*********************************************************************/
#ifndef __QUAT_H__
#define __QUAT_H__

#include "math_fwd.h"
#include "vector3.h"
#include "matrix44.h"

namespace sims
{
	class Quat
	{
	public:
		float x, y, z, w;

		Quat() : x(0), y(0), z(0), w(0) {}
		Quat(float _x, float _y, float _z, float _w)
			: x(_x), y(_y), z(_z), w(_w) {}
		Quat(const Quat& q)
			: x(q.x), y(q.y), z(q.z), w(q.w) {}
		Quat(const float* q) { memcpy(this, q, sizeof(float) * 4); }

		bool operator==(const Quat& q) const { return equal_t(x, q.x) && equal_t(y, q.y) && equal_t(z, q.z) && equal_t(w, q.w); }
		bool operator!=(const Quat& q) const { return !operator==(q); }

		Quat& operator+=(const Quat& q) { x += q.x; y += q.y; z += q.z; w += q.w; return *this; }
		Quat operator+(const Quat& q) const { Quat _q = *this; _q += q; return _q; }

		Quat& operator-=(const Quat& q) { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return *this; }
		Quat operator-(const Quat& q) const { Quat _q = *this; _q -= q; return _q; }

		Quat operator-() const { return Quat(-x, -y, -z, -w); }

		Quat& operator*=(float k) { x *= k; y *= k; z *= k; w *= k; return *this; }
		Quat operator*(float k) const { Quat q = *this; q *= k; return q; }

		Quat& operator/=(float k) { float f = 1.0f / k; x *= f; y *= f; z *= f; w *= f; return *this; }
		Quat operator/(float k) const { Quat q = *this; q /= k; return q; }

		//  [w1 v1] [w2 v2]
		// =[w1w2 - v1.v2   w1v2 + w2v1 + v1xv2]
		Quat operator*(const Quat& q) const
		{
			Quat _q;

			_q.w = w * q.w - x * q.x - y * q.y - z * q.z;
			_q.x = w * q.x + x * q.w + z * q.y - y * q.z;
			_q.y = w * q.y + y * q.w + x * q.z - z * q.x;
			_q.z = w * q.z + z * q.w + y * q.x - x * q.y;
			return _q;
		}

		Quat& operator*=(const Quat& q)
		{
			*this = *this * q;
			return *this;
		}

		float Length() const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}
		float LengthSQ() const
		{
			return x * x + y * y + z * z + w * w;
		}

		void Identity()
		{
			w = 1.0f;
			x = y = z = 0.0f;
		}

		void Normalize()
		{
			float len = Length();
			if (len > 0.0f)
			{
				float f = 1.0f / len;
				x *= f;
				y *= f;
				z *= f;
				w *= f;
			}
			else
			{
				Identity();
			}
		}
		bool IsNormalized() const { return equal_t(LengthSQ(), 1.0f); }

		void ToAxisAngle(Vector3f& v, float& angle) const
		{
			angle = float(safe_acos(w) * 2.0f * 180 / M_PI);

			// [w x y z] = [cos(angle / 2) sin(angle / 2)v]
			// w = cos(angle / 2)  sin(angle /2)^2 + cos(angle /2)^2 = 1;

			float s = 1.0f - w * w;
			if (s <= 0.0f)
				v = Vector3f(1.0f, 0.0f, 0.0f);
			else
			{
				float t = 1.0f / sqrt(s);
				v = Vector3f(x * t, y * t, z * t);
			}
		}

		string ToString() const
		{
			ostringstream oss;
			oss << "(" << x << "," << y << "," << z << "," << w << ")";
			return oss.str();
		}
	};

	inline float QuatDot(const Quat& q, const Quat& _q)
	{
		return q.w * _q.w + q.x * _q.x + q.y * _q.y + q.z * _q.z;
	}

	inline Quat QuatConjugate(const Quat& q)
	{
		return Quat(-q.x, -q.y, -q.z, q.w);
	}

	inline Quat QuatInverse(const Quat& q)
	{
		Quat _q = QuatConjugate(q);
		_q /= _q.Length();
		return _q;
	}

	// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
	inline Quat QuatLn(const Quat& q)
	{
		Vector3f v;
		float theta = 0.0f;
		q.ToAxisAngle(v, theta);
		float alpha = theta / 2.0f;
		return Quat(alpha * v.x, alpha * v.y, alpha * v.z, 0);
	}

	inline Quat QuatPow(const Quat& q, float exp)
	{
		if (ABS(q.w) > 0.9999f)
			return q;

		float alpha = acos(q.w);
		float theta = alpha * exp;
		float f = sin(theta) / sin(alpha);

		Quat _q;
		_q.w = cos(theta);
		_q.x = q.x * f;
		_q.y = q.y * f;
		_q.z = q.z * f;

		return _q;
	}

	// Spherical linear interpolation between Q0 (t == 0) and Q1 (t == 1).
	// Expects unit quaternions.
	// Slerp(q0, q1, t) = (sin(1 - t)w / sinw)q0 + (sintw / sinw)q1  cosw = q0 ¡¤ q1 / |q0||q1|
	inline Quat QuatSlerp(const Quat& q0, const Quat& q1, float t)
	{
		if (t <= 0.0f)
			return q0;
		if (t >= 1.0f)
			return q1;

		float cosw = QuatDot(q0, q1);
		float w1 = q1.w;
		float x1 = q1.x;
		float y1 = q1.y;
		float z1 = q1.z;

		if (cosw < 0.0f)
		{
			w1 = -w1;
			x1 = -x1;
			y1 = -y1;
			z1 = -z1;
			cosw = -cosw;
		}

		assert(cosw <= 1.1f);

		float k0 = 0.0f;
		float k1 = 0.0f;
		if (cosw > 0.9999f)
		{
			k0 = 1.0f - t;
			k1 = t;
		}
		else
		{
			float sinw = sqrt(1.0f - cosw * cosw);
			float w = atan2(sinw, cosw);
			float f = 1.0f / sinw;

			k0 = sin((1.0f - t) * w) * f;
			k1 = sin(t * w) * f;
		}

		Quat q;
		q.x = k0 * q0.x + k1 * x1;
		q.y = k0 * q0.y + k1 * y1;
		q.z = k0 * q0.z + k1 * z1;
		q.w = k0 * q0.w + k1 * w1;
		return q;
	}

	// v must be normal Vector
	inline Quat QuatRotationAxis(const Vector3f& v, float radian)
	{
		assert(equal_t(v.LengthSQ(), 1.0f));

		float thetaOver2 = radian * 0.5f;
		float sinThetaOver2 = sin(thetaOver2);

		Quat q;
		q.w = cos(thetaOver2);
		q.x = v.x * sinThetaOver2;
		q.y = v.y * sinThetaOver2;
		q.z = v.z * sinThetaOver2;
		return q;
	}

	// "The Shortest Arc Quaternion", Stan Melax, Game Programming Gems.
	// from v0 to v1
	inline Quat QuatRotationArc(Vector3f v0, Vector3f v1)
	{
		v0.Normalize();
		v1.Normalize();
		Vector3f c = CrossProduct(v0, v1);
		float d = DotProduct(v0, v1);
		float s = (float)sqrt((1 + d) * 2);

		Quat q;
		q.w = s / 2.0f;
		q.x = c.x / s;
		q.y = c.y / s;
		q.z = c.z / s;
		return q;
	}

	inline Quat QuatRotationX(float radian)
	{
		float thetaOver2 = radian * 0.5f;

		Quat q;
		q.w = cos(thetaOver2);
		q.x = sin(thetaOver2);
		q.y = 0.0f;
		q.z = 0.0f;
		return q;
	}

	inline Quat QuatRotationY(float radian)
	{
		float thetaOver2 = radian * 0.5f;

		Quat q;
		q.w = cos(thetaOver2);
		q.x = 0.0f;
		q.y = sin(thetaOver2);
		q.z = 0.0f;
		return q;
	}

	inline Quat QuatRotationZ(float radian)
	{
		float thetaOver2 = radian * 0.5f;

		Quat q;
		q.w = cos(thetaOver2);
		q.x = 0.0f;
		q.y = 0.0f;
		q.z = sin(thetaOver2);
		return q;
	}

	inline Quat operator*(const Quat& q, const Vector3f& v)
	{
		Quat p(v.x, v.y, v.z, 0);
		return q * p;
	}

	inline Vector3f QuatRotateVector(const Vector3f& v, const Vector3f& axis, float radian)
	{
		Vector3f _v;

		Quat q = QuatRotationAxis(axis, radian);
		Quat qInverse = QuatInverse(q);
		Quat p = q * v * qInverse;
		_v.x = p.x;
		_v.y = p.y;
		_v.z = p.z;
		return _v;
	}

	template <typename T>
	inline Matrix44<T>& MatrixRotationQuat(Matrix44<T>& n, const Quat& q)
	{
		// Common expr

		float _2x = 2.0f * q.x;
		float _2y = 2.0f * q.y;
		float _2z = 2.0f * q.z;

		float _2xy = _2x * q.y;
		float _2yz = _2y * q.z;
		float _2xz = _2z * q.x;

		float _2wx = _2x * q.w;
		float _2wy = _2y * q.w;
		float _2wz = _2z * q.w;

		n.ZeroTranslation();

		n.m11 = 1.0f - _2y * q.y - _2z * q.z;
		n.m12 = _2xy + _2wz;
		n.m13 = _2xz - _2wy;
		n.m14 = 0.0f;

		n.m21 = _2xy - _2wz;
		n.m22 = 1.0f - _2x * q.x - _2z * q.z;
		n.m23 = _2yz + _2wx;
		n.m24 = 0.0f;

		n.m31 = _2xz + _2wy;
		n.m32 = _2yz - _2wx;
		n.m33 = 1.0f - _2x * q.x - _2y * q.y;
		n.m34 = 0.0f;

		n.m44 = 1.0f;

		return n;
	}
}

#endif