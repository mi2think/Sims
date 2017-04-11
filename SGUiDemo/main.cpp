#include "sims.h"
#include "gui/gui.h"
#include "utils/demo_app.h"
#include "math/matrix44.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

class SGuiDemo : public DemoApp<HWWindow>
{
public:
	SGuiDemo()
		: renderer_(hw::GetRenderer())
	{
	}

	~SGuiDemo()
	{
	}

	virtual void OnCreate()
	{
		Vector3f pos(0.0f, 3.0f, -5.0f);
		Vector3f target(0.0f, 0.0f, 0.0f);
		Vector3f up(0.0f, 1.0f, 0.0f);
		Matrix44f view;
		MatrixLookAtLH(view, pos, target, up);
		renderer_->SetTransform(Transform::View, view);

		// projection
		Matrix44f proj;
		MatrixPerspectiveFovLH(proj,
			M_PI / 4.0f,
			(float)width_ / height_,
			1.0f,
			100.0f);
		renderer_->SetTransform(Transform::Projection, proj);
	}

	virtual void OnRender(const Timestep& timestep)
	{
		if (renderer_)
		{
			// draw
			renderer_->BeginFrame(ClearFlags::Color | ClearFlags::Depth, Color(0xff0000ff), 1.0f, 0);

			renderer_->EndFrame();
			renderer_->PresentFrame();
		}
	}

	virtual void OnDestroy()
	{
	}

	hw::HWRenderer* renderer_;
};

int main()
{
	SGuiDemo app;
	app.Create(800, 600, "SGuiDemo");
	app.Loop();
	return 0;
}