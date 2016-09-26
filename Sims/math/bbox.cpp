/********************************************************************
	created:	2016/09/16
	created:	16:9:2016   22:28
	filename: 	D:\Code\Sims\Sims\math\bbox.cpp
	file path:	D:\Code\Sims\Sims\math
	file base:	bbox
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Bounding Box
*********************************************************************/
#include "bbox.h"
#include "vector4.h"
#include "matrix44.h"
#include "ray.h"

namespace sims
{
	bool BBox::Overlaps(const BBox& bbox) const
	{
		bool x = maxP_.x >= bbox.minP_.x && minP_.x <= maxP_.x;
		bool y = maxP_.y >= bbox.minP_.y && minP_.y <= maxP_.y;
		bool z = maxP_.z >= bbox.minP_.z && minP_.z <= maxP_.z;
		return x && y && z;
	}

	BBox& BBox::Union(const Vector3f& pt)
	{
		if (pt.x < minP_.x) minP_.x = pt.x;
		if (pt.y < minP_.y) minP_.y = pt.y;
		if (pt.z < minP_.z) minP_.z = pt.z;

		if (pt.x > maxP_.x) maxP_.x = pt.x;
		if (pt.y > minP_.y) maxP_.y = pt.y;
		if (pt.z > minP_.z) maxP_.z = pt.z;

		return *this;
	}

	BBox& BBox::Union(const BBox& bbox)
	{
		if (&bbox != this)
		{
			if (bbox.minP_.x < minP_.x) minP_.x = bbox.minP_.x;
			if (bbox.minP_.y < minP_.y) minP_.y = bbox.minP_.y;
			if (bbox.minP_.z < minP_.z) minP_.z = bbox.minP_.z;

			if (bbox.maxP_.x > maxP_.x) maxP_.x = bbox.maxP_.x;
			if (bbox.maxP_.y > maxP_.y) maxP_.y = bbox.maxP_.y;
			if (bbox.maxP_.z > maxP_.z) maxP_.z = bbox.maxP_.z;
		}

		return *this;
	}

	BBox& BBox::Transform(const Matrix44f& m)
	{
		Reset();

		Vector4f points[] = {
			{ minP_.x, minP_.y, minP_.z, 1 },
			{ maxP_.x, minP_.y, minP_.z, 1 },
			{ minP_.x, maxP_.y, minP_.z, 1 },
			{ minP_.x, minP_.y, maxP_.z, 1 },
			{ minP_.x, maxP_.y, maxP_.z, 1 },
			{ maxP_.x, maxP_.y, minP_.z, 1 },
			{ maxP_.x, minP_.y, maxP_.z, 1 },
			{ maxP_.x, maxP_.y, maxP_.z, 1 }
		};

		for (auto& pt : points)
		{
			pt = pt * m;
			Union(pt.DivW());
		}
		return *this;
	}

	bool Intersect(const Ray& ray, const BBox& bbox, float& t)
	{
		float t0 = ray.mint_;
		float t1 = ray.maxt_;

		for (int i = 0; i < 3; ++i)
		{
			float invRayDir = 1.0f / ray.direction_[i];
			float tnear = (bbox.minP_[i] - ray.origin_[i]) * invRayDir;
			float tfar = (bbox.maxP_[i] - ray.origin_[i]) * invRayDir;
			if (tnear > tfar)
				swap_t(tnear, tfar);
			t0 = tnear > t0 ? tnear : t0;
			t1 = tfar < t1 ? tfar : t1;
			if (t0 > t1)
				return false;
		}
		t = t0;
		return true;
	}
}