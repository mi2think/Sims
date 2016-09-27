/********************************************************************
	created:	2016/09/27
	created:	27:9:2016   10:52
	filename: 	D:\Code\Sims\Sims\math\ray.cpp
	file path:	D:\Code\Sims\Sims\math
	file base:	ray
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Ray
*********************************************************************/
#include "ray.h"
#include "matrix44.h"

namespace sims
{
	Ray GetViewRay(int screenX, 
		int screenY, 
		int screenWidth, 
		int screenHeight, 
		const Matrix44f& proj)
	{
		/** object -> world -> view -> projection -> viewport -> screen
		 *                       ?  <-            <-           <- known
		 *  so we have screen coord, target is point in view.
		 *  view matrix:
		 *	[Width/2   0         0       0]
		 *	[0      -Height/2    0       0]
		 * 	[0         0      MaxZ-MinZ  0]
		 *	[Width/2 Height/2   MinZ     1]
		 *
		 *  projection matrix:
		 *  [cot(theta)/aspect      0        0     0]
		 *  [cot(theta)/aspect cot(theta)    0     0]
		 *  [          0            0    f/(f - n) 1]
		 *  [          0            0  f*n/(n - f) 1]
		 **/

		float x = ( 2.0f * screenX / screenWidth  - 1.0f) / proj.m11;
		float y = (-2.0f * screenY / screenHeight + 1.0f) / proj.m22;
		float z = 1.0f; // near plane
		Vector3f dir(x, y, z);
		dir.Normalize();

		return Ray(Vector3f(0.0f, 0.0f, 0.0f), dir);
	}

	Ray TransformRay(const Ray& ray, const Matrix44f& m)
	{
		Vector3f origin = ray.origin_;
		origin = origin * m;

		Vector3f direction = ray.direction_;
		direction = TransformVec3(direction, m);
		direction.Normalize();

		return Ray(origin, direction, ray.mint_, ray.maxt_);
	}
}
