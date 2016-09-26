/********************************************************************
	created:	2016/09/26
	created:	26:9:2016   22:52
	filename: 	D:\Code\Sims\Sims\math\ray.h
	file path:	D:\Code\Sims\Sims\math
	file base:	ray
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Ray
*********************************************************************/
#ifndef __RAY_H__
#define __RAY_H__

#include "math_fwd.h"
#include "vector3.h"

namespace sims
{
	class Ray
	{
	public:
		Ray() : mint_(0.0f), maxt_(FLT_MAX) {}
		Ray(const Vector3f& origin, const Vector3f& direction,
			float mint, float maxt = FLT_MAX)
			: origin_(origin)
			, direction_(direction)
			, mint_(mint)
			, maxt_(maxt)
		{}

		Vector3f PointAt(float t) const { return origin_ + direction_ * t; }

		// r(t) = origin_ + t * direction_, t >= 0
		Vector3f origin_;
		Vector3f direction_;

		// restrict ray to [r(mint), r(maxt)]
		mutable float mint_;
		mutable float maxt_;
	};
}

#endif
