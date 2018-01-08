#include "sims.h"
#include "math/math.h"
#include "core/app.h"
#include "graphics/vertex_buffer.h"
#include "graphics/shader.h"
#include "graphics/program.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

class UniformVariables : public App<HWWindow>
{
public:
	virtual void OnCreate()
	{
		renderer_ = hw::GetRenderer();

		SetupVertexBuffer();
		SetupProg();
	}

	virtual void OnRender(const Timestep&)
	{
		if (renderer_)
		{
			static float scale = 0.0f;
			scale += 0.001f;

			hw::SetUniform(prog_, "gScale", sinf(scale), ShaderDomain::Vertex);

			// draw
			renderer_->BeginFrame(ClearFlags::Color | ClearFlags::Depth, Color(0xff000000), 1.0f, 0);

			vertexDecl_->HWResource()->BindResource();
			vertexBuf_->HWResource()->BindResource();
			renderer_->DrawPrimitive(PrimitiveType::Triangles, 1);

			renderer_->EndFrame();
			renderer_->PresentFrame();
		}
	}

	void SetupVertexBuffer()
	{
		VertexStream stream
		(
			0,
			VertexElement(VertexElementUsage::Position, 0, DataType::F32, 3),
			VertexElement(VertexElementUsage::Color, 0, DataType::F32, 3)
		);
		vertexDecl_ = VertexDeclaration::Get(&stream, 1);
		vertexDecl_->Invalidate();

		vertexBuf_ = new VertexBuffer(vertexDecl_, 3, 0);
		auto L = vertexBuf_->Lock(LockFlags::LockWrite);
		struct Vertex
		{
			Vector3f P;
			Vector3f C;
		};
		Vertex v[3] =
		{
			Vertex{ Vector3f(-1.0f, -1.0f, 0.0f), Color::c_red.GetRGBVector3() },
			Vertex{ Vector3f(0.0f, 1.0f, 0.0f),   Color::c_green.GetRGBVector3() },
			Vertex{ Vector3f(1.0f, -1.0f, 0.0f),  Color::c_blue.GetRGBVector3() },
		};
		memcpy(L->GetData(), &v[0], sizeof(v));
		vertexBuf_->Unlock(L);
		vertexBuf_->Invalidate();
	}

	void SetupProg()
	{
		ShaderRef VS = new Shader(ShaderDomain::Vertex);
		ShaderRef PS = new Shader(ShaderDomain::Fragment);
#if SIMS_SDK_IMPL_OGL
		VS->SetSourceFromFile("../res/05_UniformVariables/ogl/vs.glsl");
		PS->SetSourceFromFile("../res/05_UniformVariables/ogl/ps.glsl");
#elif SIMS_SDK_IMPL_D3D9
		VS->SetSourceFromFile("../res/05_UniformVariables/dx9/vs.hlsl");
		PS->SetSourceFromFile("../res/05_UniformVariables/dx9/ps.hlsl");
#endif
		VS->SetEntryName("main");
		PS->SetEntryName("main");
		VS->Invalidate();
		PS->Invalidate();

		prog_ = new Program();
		prog_->AddShader(VS);
		prog_->AddShader(PS);
		prog_->Invalidate();
		prog_->HWResource()->BindResource();
	}
private:
	hw::HWRenderer* renderer_;
	VertexDeclarationRef vertexDecl_;
	VertexBufferRef vertexBuf_;
	ProgramRef prog_;
};

int main()
{
	UniformVariables app;
	app.Create(640, 480, "Uniform Variables");
	app.Loop();
	return 0;
}