#include "sims.h"
#include "math/math.h"
#include "core/app.h"
#include "graphics/vertex_array.h"
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
			static float scale = 0.0f;
			scale += 0.001f;

			// built mat, note, opengl matrix is col major, in glsl, use matrix * vector
			// in hlsl, use vector * materix

			// !! Note: In clip space, ogl z: [-1, 1]; dx z: [ 0, 1].
			// so here we should not over z range.

			Matrix44f mat;
			Matrix44f m1;
			MatrixRotationY(m1, scale);
			Matrix44f m2;
			MaxtrixTranslation(m2, Vector3f(0.0f, 0.0f, 0.5f));
			mat = m1 * m2;

			hw::SetUniform(prog_, "gMatWorld", mat, ShaderDomain::Vertex);

			// draw
			renderer_->BeginFrame(ClearFlags::Color | ClearFlags::Depth, Color(0xff000000), 1.0f, 0);

			vertexArray_->Bind();
			renderer_->DrawIndexedPrimitive(PrimitiveType::Triangles, IndexBuf_->Resource(), 4, 4);

			renderer_->EndFrame();
			renderer_->PresentFrame();
		}
	}

	void SetupVertexBuffer()
	{
		VertexStream streams[2] =
		{
			VertexStream(VertexElement(VertexElementUsage::Position, 0, DataType::F32, 3), 0),
			VertexStream(VertexElement(VertexElementUsage::Color, 0, DataType::F32, 3), 1),
		};
		VertexDeclarationRef vertexDecl = VertexDeclaration::Get(&streams[0], 2);

		// vertex buffer 1
		VertexBufferRef vertexBuf1 = new VertexBuffer(vertexDecl, 4, 0);
		auto L1 = vertexBuf1->Lock(LockFlags::LockWrite);
		Vector3f v1[4] =
		{
			Vector3f(-0.5f, -0.5f, 0.0f),
			Vector3f(0.0f, -0.5f, 0.5f),
			Vector3f(0.5f, -0.5f, 0.0f),
			Vector3f(0.0f, 0.5f, 0.0f),
		};
		memcpy(L1->GetData(), &v1[0], sizeof(v1));
		vertexBuf1->Unlock(L1);

		// vertex buffer 2
		VertexBufferRef vertexBuf2 = new VertexBuffer(vertexDecl, 4, 1);
		auto L2 = vertexBuf2->Lock(LockFlags::LockWrite);
		Vector3f v2[4] =
		{
			Vector3f(1.0f, 0.0f, 0.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector3f(1.0f, 1.0f, 1.0f),
		};
		memcpy(L2->GetData(), &v2[0], sizeof(v2));
		vertexBuf2->Unlock(L2);

		vertexArray_ = new VertexArray(vertexDecl, vertexBuf1, vertexBuf2);
		vertexArray_->Invalidate();
	}

	void SetupIndexBuffer()
	{
		uint16 indices[] = { 0, 1, 3,
			1, 2, 3,
			2, 0, 3,
			0, 2, 1 };

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
		prog_->Bind();
	}
private:
	hw::HWRenderer* renderer_;
	VertexArrayRef vertexArray_;
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