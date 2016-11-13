/********************************************************************
	created:	2016/09/26
	created:	26:9:2016   23:10
	filename: 	D:\Code\Sims\Sims\math\sphere.h
	file path:	D:\Code\Sims\Sims\math
	file base:	sphere
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Sphere
*********************************************************************/
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "math_fwd.h"
#include "vector3.h"
#include "ray.h"

namespace sims
{
	class Sphere
	{
	public:
		Sphere() : radius_(0.0f) {}
		Sphere(const Vector3f& center, float radius) 
			: center_(center)
			, radius_(radius)
		{}

		Vector3f center_;
		float radius_;
	};

	inline bool Intersect(const Sphere& sphere, const Ray& ray, float& t)
	{
		// compute quadratic sphere coefficients
		const auto& o = ray.origin_;
		const auto& d = ray.direction_;
		Vector3f temp = o - sphere.center_;
		float A = DotProduct(d, d);
		float B = 2 * DotProduct(d, temp);
		float C = DotProduct(temp, temp) - sphere.radius_ * sphere.radius_;

		// solve quadratic equation for t values
		float t0;
		float t1;
		if (!quadratic(A, B, C, &t0, &t1))
			return false;

		// compute intersection distance along ray
		if (t0 > ray.maxt_ || t1 < ray.mint_)
			return false;

		t = t0;
		if (t < ray.mint_)
		{
			t = t1;
			if (t > ray.maxt_)
				return false;
		}
		return true;
	}
}


#endif
