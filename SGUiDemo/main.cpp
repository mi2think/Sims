#include "sims.h"
#include "gui/gui.h"
#include "utils/demo_app.h"
#include "graphics_api/sims_sdk_d3d9.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

#define SIMS_SDK_IMPL_D3D9 1

class SGuiDemo : public DemoApp<d3d9::Window>
{
public:
	SGuiDemo()
	{
	}

	~SGuiDemo()
	{
	}

	virtual void OnCreate()
	{
		// view
		D3DXVECTOR3 pos(0.0f, 3.0f, -5.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX view;
		D3DXMatrixLookAtLH(&view, &pos, &target, &up);
		d3d9::CHECK_HR = d3d9::g_pD3DD->SetTransform(D3DTS_VIEW, &view);

		// projection
		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj,
			D3DX_PI / 4.0f,
			(float)width_ / height_,
			1.0f,
			100.0f);
		d3d9::CHECK_HR = d3d9::g_pD3DD->SetTransform(D3DTS_PROJECTION, &proj);

		d3d9::CHECK_HR = d3d9::g_pD3DD->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

		auto render = hw::GetRenderer();
	}

	virtual void OnRender(const Timestep& timestep)
	{
		if (d3d9::g_pD3DD)
		{
			// draw
			d3d9::CHECK_HR = d3d9::g_pD3DD->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0);
			d3d9::CHECK_HR = d3d9::g_pD3DD->BeginScene();


			d3d9::CHECK_HR = d3d9::g_pD3DD->EndScene();
			d3d9::CHECK_HR = d3d9::g_pD3DD->Present(0, 0, 0, 0);
		}
	}

	virtual void OnDestroy()
	{
	}
};

int main()
{
	SGuiDemo app;
	app.Create(800, 600, "SGuiDemo");
	app.Loop();
	return 0;
}