/********************************************************************
	created:	2016/11/13
	created:	13:11:2016   23:15
	filename: 	D:\Code\Sims\Sims\math\frustum.cpp
	file path:	D:\Code\Sims\Sims\math
	file base:	frustum
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Frustum
*********************************************************************/
#include "frustum.h"
#include "matrix44.h"

namespace sims
{
	Frustum::Frustum(const Matrix44f& view, const Matrix44f& proj)
	{
		ExtractFrustum(view, proj);
	}

	Frustum::Frustum(const Matrix44f& viewProj)
	{
		ExtractFrustum(viewProj);
	}

	const Plane& Frustum::GetPlane(PlaneID index) const
	{
		assert(index < PlaneMax);
		return planes_[index];
	}

	void Frustum::ExtractFrustum(const Matrix44f& viewProj)
	{
		//Fast Extraction of Viewing Frustum Planes - Gribb & Hartmann
		//http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf

		const auto& m = viewProj.m;

		// Near clipping plane
		planes_[Frustum::PlaneNear] = Plane(m[0][2],
			m[1][2],
			m[2][2],
			m[3][2]);

		// Far clipping plane
		planes_[Frustum::PlaneFar] = Plane(m[0][3] - m[0][2],
			m[1][3] - m[1][2],
			m[2][3] - m[2][2],
			m[3][3] - m[3][2]);

		//left clipping plane
		planes_[Frustum::PlaneLeft] = Plane(m[0][3] + m[0][0],
			m[1][3] + m[1][0],
			m[2][3] + m[2][0],
			m[3][3] + m[3][0]);

		// Right clipping plane
		planes_[Frustum::PlaneRight] = Plane(m[0][3] - m[0][0],
			m[1][3] - m[1][0],
			m[2][3] - m[2][0],
			m[3][3] - m[3][0]);

		// Top clipping plane
		planes_[Frustum::PlaneTop] = Plane(m[0][3] - m[0][1],
			m[1][3] - m[1][1],
			m[2][3] - m[2][1],
			m[3][3] - m[3][1]);

		// Bottom clipping plane
		planes_[Frustum::PlaneBottom] = Plane(m[0][3] + m[0][1],
			m[1][3] + m[1][1],
			m[2][3] + m[2][1],
			m[3][3] + m[3][1]);
	}

	void Frustum::ExtractFrustum(const Matrix44f& view, const Matrix44f& proj)
	{
		Matrix44f viewProj;
		MatrixMultiply(viewProj, view, proj);
		ExtractFrustum(viewProj);
	}

	bool Frustum::Inside(const Vector3f& pt) const
	{
		for (int i = 0; i < Frustum::PlaneMax; i++)
		{
			if (planes_[i].Distance(pt) < 0)
				return false;
		}
		return true;
	}
}