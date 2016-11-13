/********************************************************************
	created:	2016/11/13
	created:	13:11:2016   23:08
	filename: 	D:\Code\Sims\Sims\math\frustum.h
	file path:	D:\Code\Sims\Sims\math
	file base:	frustum
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Frustum
*********************************************************************/
#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include "math_fwd.h"
#include "vector3.h"
#include "plane.h"

namespace sims
{
	class Frustum
	{
	public:
		enum PlaneID
		{
			PlaneNear,
			PlaneFar,
			PlaneLeft,
			PlaneRight,
			PlaneTop,
			PlaneBottom,
			PlaneMax
		};
		Plane planes_[PlaneMax];

		Frustum() = default;
		Frustum(const Matrix44f& view, const Matrix44f& proj);
		Frustum(const Matrix44f& viewProj);

		const Plane& GetPlane(PlaneID index) const;
		// extract frustum from view-projection matrix
		void ExtractFrustum(const Matrix44f& viewProj);

		bool Inside(const Vector3f& pt) const;
	};
}

#endif