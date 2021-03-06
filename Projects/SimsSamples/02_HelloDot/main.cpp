#include "sims.h"
#include "math/math.h"
#include "core/app.h"
#include "graphics/vertex_buffer.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

class HelloDot : public App<HWWindow>
{
public:
	virtual void OnCreate()
	{
		renderer_ = hw::GetRenderer();

		VertexStream stream 
		(
			VertexElement(VertexElementUsage::Position, 0, DataType::F32, 3),
			VertexElement(VertexElementUsage::Color, 0, DataType::U32, 1)
		);
		vertexDecl_ = VertexDeclaration::Get(&stream, 1);
		vertexDecl_->Invalidate();

		vertexBuf_ = new VertexBuffer(vertexDecl_, 1, 0);
		auto L = vertexBuf_->Lock(LockFlags::LockWrite);
		struct Vertex
		{
			Vector3f P;
			Color C;
		};
		Vertex v{ Vector3f(0.0f, 0.0f, 0.0f), Color::c_white };
		*((Vertex*)L->GetData()) = v;
		vertexBuf_->Unlock(L);
		vertexBuf_->Invalidate();
	}

	virtual void OnRender(const Timestep&)
	{
		if (renderer_)
		{
			// draw
			renderer_->BeginFrame(ClearFlags::Color | ClearFlags::Depth, Color(0xff000000), 1.0f, 0);

			vertexDecl_->Bind();
			vertexBuf_->Bind();
			renderer_->DrawPrimitive(PrimitiveType::Points, 1);
			
			renderer_->EndFrame();
			renderer_->PresentFrame();
		}

	}
private:
	VertexDeclarationRef vertexDecl_;
	VertexBufferRef vertexBuf_;
	hw::HWRenderer* renderer_;
};

int main()
{
	HelloDot app;
	app.Create(640, 480, "Hello Dot");
	app.Loop();
	return 0;
}