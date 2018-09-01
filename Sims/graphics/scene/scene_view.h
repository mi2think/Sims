/********************************************************************
	created:	2018/09/01
	created:	1:9:2018   22:34
	filename: 	D:\Code\Sims\Sims\graphics\scene\scene_view.h
	file path:	D:\Code\Sims\Sims\graphics\scene
	file base:	scene_view
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Scene View
*********************************************************************/
#pragma once

#include "sims.h"
#include "math/math.h"

namespace sims
{
	struct Viewport
	{
		int32 x;
		int32 y;
		int32 width;
		int32 height;
	};


	class SceneView
	{
	public:
		SceneView(const Matrix44f& view, const Matrix44f& proj);

		const Matrix44f& GetView() const { return viewM_; }
		const Matrix44f& GetProj() const { return projM_; }
		const Matrix44f& GetInversedView() const { return inversedViewM_; }
		const Matrix44f& GetInversedProj() const { return inversedProjM_; }
		const Vector3f&  GetCameraPos() const { return cameraWorldPos_; }
		const Vector3f&  GetCameraDir() const { return cameraDir_; }

		void SetViewport(int32 x, int32 y, int32 width, int32 height);
		const Viewport& GetViewport() const { return viewport_; }
	private:
		Matrix44f viewM_;
		Matrix44f inversedViewM_;
		Matrix44f projM_;
		Matrix44f inversedProjM_;
		Frustum   frustum_;
		Vector3f  cameraWorldPos_;
		Vector3f  cameraDir_;

		Viewport viewport_;
	};
}

