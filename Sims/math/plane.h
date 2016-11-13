/********************************************************************
	created:	2016/11/13
	created:	13:11:2016   22:32
	filename: 	D:\Code\Sims\Sims\math\plane.h
	file path:	D:\Code\Sims\Sims\math
	file base:	plane
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Plane
*********************************************************************/
#ifndef __PLANE_H__
#define __PLANE_H__

#include "math_fwd.h"
#include "vector3.h"
#include "ray.h"

namespace sims
{
	// for a plane, is a collection of p, which n <dot> (p - p0) = 0, then n <dot> p - n <dot> p0 = 0
	// we know: x * n.x + y * n.y + z * n.z + d = 0
	// so: d = -n.p0, then: n.p + d = 0
	class Plane
	{
	public:
		Vector3f n_; // normal
		float d_;	 // d

		Plane() = default;
		Plane(const Vector3f& n, float d);
		Plane(const Vector3f& n, const Vector3f& p);
		Plane(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2);
		Plane(float a, float b, float c, float d);

		// signed distance:
		// =0: p lies in plane
		// <0: p lies on negative side of plane
		// >0: p lies on positive side of plane
		float Distance(const Vector3f& p) const;

		// nearest point in plane
		Vector3f NearestPoint(const Vector3f& p) const;

		void Normalize();
	};

	// transform plane by matrix
	Plane TransformPlane(const Plane& plane, const Matrix44f& m);

	bool Intersect(const Plane& plane, const Ray& ray, float& t);
}

#endif
