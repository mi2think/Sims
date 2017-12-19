#include "sims.h"
#include "core/app.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

class OpenAWindow : public App<HWWindow>
{
public:
	OpenAWindow()
		: renderer_(hw::GetRenderer())
	{

	}
	virtual void OnRender(const Timestep&)
	{
		if (renderer_)
		{
			// draw
			renderer_->BeginFrame(ClearFlags::Color | ClearFlags::Depth, Color(0xff0000ff), 1.0f, 0);

			renderer_->EndFrame();
			renderer_->PresentFrame();
		}
	}

	hw::HWRenderer* renderer_;
};

int main()
{
	OpenAWindow app;
	app.Create(640, 480, "Open A Window");
	app.Loop();
	return 0;
}