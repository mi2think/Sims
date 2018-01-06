#include "sims.h"
#include "math/math.h"
#include "core/app.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"
#include "graphics/shader.h"
#include "graphics/program.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

class IndexedDraws : public App<HWWindow>
{
public:
	virtual void OnCreate()
	{
		renderer_ = hw::GetRenderer();

		SetupVertexBuffer();
		SetupIndexBuffer();
		SetupProg();
	}

	virtual void OnRender(const Timestep&)
	{
		if (renderer_)
		{
			// draw
			renderer_->BeginFrame(ClearFlags::Color | ClearFlags::Depth, Color(0xff000000), 1.0f, 0);

			vertexBuf1_->HWResource()->BindResource();
			vertexBuf2_->HWResource()->BindResource();
			renderer_->DrawIndexedPrimitive(PrimitiveType::Triangles, *IndexBuf_->HWResource(), 4, 4);

			renderer_->EndFrame();
			renderer_->PresentFrame();
		}
	}

	void SetupVertexBuffer()
	{
		VertexStream streams[2] =
		{
			VertexStream(0, VertexElement(VertexElementUsage::Position, 0, DataType::F32, 3)),
			VertexStream(1, VertexElement(VertexElementUsage::Color, 0, DataType::F32, 3)),
		};
		VertexDeclarationRef vertexDecl = VertexDeclaration::Get(&streams[0], 2);

		// vertex buffer 1
		vertexBuf1_ = new VertexBuffer(vertexDecl, 4, 0);
		auto L1 = vertexBuf1_->Lock(LockFlags::LockWrite);
		Vector3f v1[4] =
		{
			Vector3f(-1.0f, -1.0f, 0.0f),
			Vector3f(0.0f, -1.0f, 1.0f),
			Vector3f(1.0f, -1.0f, 0.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
		};
		memcpy(L1->GetData(), &v1[0], sizeof(v1));
		vertexBuf1_->Unlock(L1);
		vertexBuf1_->Invalidate();

		// vertex buffer 2
		vertexBuf2_ = new VertexBuffer(vertexDecl, 4, 1);
		auto L2 = vertexBuf2_->Lock(LockFlags::LockWrite);
		Vector3f v2[4] =
		{
			Color::c_red.GetRGBVector3(),
			Color::c_green.GetRGBVector3(),
			Color::c_blue.GetRGBVector3(),
			Color::c_white.GetRGBVector3(),
		};
		memcpy(L2->GetData(), &v2[0], sizeof(v2));
		vertexBuf2_->Unlock(L2);
		vertexBuf2_->Invalidate();
	}

	void SetupIndexBuffer()
	{
		uint16 indices[] = { 0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 1, 2 };

		IndexBuf_ = new IndexBuffer(ARRAY_COUNT(indices), &indices[0]);
		IndexBuf_->Invalidate();
	}

	void SetupProg()
	{
		ShaderRef VS = new Shader(ShaderDomain::Vertex);
		ShaderRef PS = new Shader(ShaderDomain::Fragment);
#if SIMS_SDK_IMPL_OGL
		VS->SetSourceFromFile("../res/10_IndexedDraws/ogl/vs.glsl");
		PS->SetSourceFromFile("../res/10_IndexedDraws/ogl/ps.glsl");
#elif SIMS_SDK_IMPL_D3D9
		VS->SetSourceFromFile("../res/10_IndexedDraws/dx9/vs.hlsl");
		PS->SetSourceFromFile("../res/10_IndexedDraws/dx9/ps.hlsl");
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
	VertexBufferRef vertexBuf1_;
	VertexBufferRef vertexBuf2_;
	IndexBufferRef IndexBuf_;
	ProgramRef prog_;
};

int main()
{
	IndexedDraws app;
	app.Create(640, 480, "Indexed Draws");
	app.Loop();
	return 0;
}