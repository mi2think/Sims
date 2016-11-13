/********************************************************************
	created:	2016/11/13
	created:	13:11:2016   22:41
	filename: 	D:\Code\Sims\Sims\math\plane.cpp
	file path:	D:\Code\Sims\Sims\math
	file base:	plane
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Plane
*********************************************************************/
#include "plane.h"
#include "matrix44.h"

namespace sims
{
	Plane::Plane(const Vector3f& n, float d)
		: n_(n)
		, d_(d)
	{
		assert(n.IsNormalized());
	}

	Plane::Plane(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2)
	{
		Vector3f u = p1 - p0;
		Vector3f v = p2 - p0;
		n_ = CrossProduct(u, v);
		n_.Normalize();
		d_ = -DotProduct(n_, p0);
	}

	Plane::Plane(const Vector3f& n, const Vector3f& p)
		: n_(n)
	{
		assert(n_.IsNormalized());
		d_ = -DotProduct(n_, p);
	}

	Plane::Plane(float a, float b, float c, float d)
		: n_(a, b, c)
		, d_(d)
	{
		Normalize();
	}

	float Plane::Distance(const Vector3f& p) const
	{
		return DotProduct(n_, p) + d_;
	}

	Vector3f Plane::NearestPoint(const Vector3f& p) const
	{
		// we assume q is the nearest point in plane for p
		// and k is the nearest distance from p to plane. since n is unit-vector
		// such: q = p + (-kn), and k = Distance(p)
		return p - Distance(p) * n_;
	}

	void Plane::Normalize()
	{
		float len = n_.Length();
		float f = 1.0f / len;
		n_ *= f;
		d_ *= f; // we also need divide d by len
	}

	Plane TransformPlane(const Plane& plane, const Matrix44f& m)
	{
		// transform plane as vector4, see formula in page 102
		Vector4f v(plane.n_.x, plane.n_.y, plane.n_.z, plane.d_);
		v *= m;
		return Plane(Vector3f(v.x, v.y, v.z), v.w);
	}

	bool Intersect(const Plane& plane, const Ray& ray, float& t)
	{
		//see formula in page 99
		float dp = DotProduct(plane.n_, ray.direction_);
		if (equal_t(dp, 0.0f))
		{
			// ray is parallel to plane
			// may the ray lies in the plane if DotProduct(plane.n_, ray.pt_) + plane.d_ == 0
			// we treat this as not intersect yet.
			return false;
		}

		float k = DotProduct(plane.n_, ray.origin_);
		float t0 = -(k + plane.d_) / dp;
		if (t0 < 0)
			return false;
		t = t0;
		return true;
	}
}
