/********************************************************************
	created:	2018/09/01
	created:	1:9:2018   22:54
	filename: 	D:\Code\Sims\Sims\graphics\scene\scene_view.cpp
	file path:	D:\Code\Sims\Sims\graphics\scene
	file base:	scene_view
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Scene View
*********************************************************************/
#include "scene_view.h"

namespace sims
{
	SceneView::SceneView(const Matrix44f& view, const Matrix44f& proj)
		: viewM_(view)
		, projM_(proj)
	{
		MatrixInverse(inversedViewM_, viewM_);
		MatrixInverse(inversedProjM_, projM_);

		cameraWorldPos_ = (Vector4f(0, 0, 0, 1) * inversedViewM_).GetXYZ();
		cameraDir_ = (Vector4f(0, 0, 1, 0) * inversedViewM_).GetXYZ();
	}
}
