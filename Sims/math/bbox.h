/********************************************************************
	created:	2016/09/16
	created:	16:9:2016   22:22
	filename: 	D:\Code\Sims\Sims\math\bbox.h
	file path:	D:\Code\Sims\Sims\math
	file base:	bbox
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Bounding Box/Sphere
*********************************************************************/
#ifndef __BBOX_H__
#define __BBOX_H__

#include "math_fwd.h"
#include "vector3.h"

namespace sims
{
	class BBox
	{
	public:
		Vector3f minP_;
		Vector3f maxP_;

		BBox() { Reset(); }
		BBox(const Vector3f& mi, const Vector3f& ma) : minP_(mi), maxP_(ma) {}
		BBox(const Vector3f& c, float radius) { Vector3f r(radius, radius, radius); minP_ = c - r; maxP_ = c + r; } // may restore sphere by InnerSphere
		BBox(const BBox& bbox) : minP_(bbox.minP_), maxP_(bbox.maxP_) {}
		BBox& operator=(const BBox& bbox) { minP_ = bbox.minP_; maxP_ = bbox.maxP_; return *this; }
		BBox& operator|=(const BBox& bbox) { return Union(bbox); }

		bool Valid() const { return minP_.x <= maxP_.x && minP_.y <= maxP_.y && minP_.z <= maxP_.z; }
		void Reset() { minP_.x = minP_.y = minP_.z = FLT_MAX; maxP_.x = maxP_.y = maxP_.z = FLT_MIN; }
		Vector3f Size() const { return maxP_ - minP_; }
		Vector3f Center() const { return (maxP_ + minP_) * 0.5f; }
		void Expand(float delta) { Vector3f v(delta, delta, delta); minP_ -= v; maxP_ += v; }
		bool Inside(const Vector3f& pt) const
		{
			return pt.x >= minP_.x && pt.x <= maxP_.x
				&& pt.y >= minP_.y && pt.y <= maxP_.y
				&& pt.z >= minP_.z && pt.z <= maxP_.z;
		}
		bool Overlaps(const BBox& bbox) const;

		float SurfaceArea() const
		{
			Vector3f size = Size();
			return 2.f * (size.x * size.y + size.x * size.z + size.y * size.z);
		}

		float Volume() const
		{
			Vector3f size = Size();
			return size.x * size.y * size.z;
		}

		void BoundingSphere(Vector3f& c, float& radius) const
		{
			c = Center();
			radius = Inside(c) ? Distance(c, maxP_) : 0.f;
		}

		void InnerSphere(Vector3f& c, float& radius) const
		{
			c = Center();

			Vector3f aSize = Size();
			float d = aSize.x;
			if (d > aSize.y)
				d = aSize.y;
			if (d > aSize.z )
				d = aSize.z;
			radius = Inside(c) ? 0.5f * d : 0.0f;
		}

		BBox& Union(const Vector3f& pt);
		BBox& Union(const BBox& bbox);
		BBox& Transform(const Matrix44f& m);
	};

	bool Intersect(const Ray& ray, const BBox& bbox, float& t);
}

#endif
