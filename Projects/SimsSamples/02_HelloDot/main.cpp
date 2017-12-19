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

		VertexStream streams[2] = 
		{ 
			VertexStream(VertexStreamUsage::Position, 0, ElementType::F32, 3),
			VertexStream(VertexStreamUsage::Color, 0, ElementType::U32, 1)
		};
		VertexDeclarationRef vertexDecl = VertexDeclaration::Get(&streams[0], 2);
		vertexBuf_ = new VertexBuffer(vertexDecl, 1);
		vertexBuf_->SetStorageFlags(StorageFlags::Hardware);

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

			renderer_->DrawPrimitive(PrimitiveType::Points, *(vertexBuf_->HWResource()), 1);
			
			renderer_->EndFrame();
			renderer_->PresentFrame();
		}

	}
private:
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