#include "sims.h"
#include "math/math.h"
#include "utils/demo_app.h"
#include "graphics/vertex_buffer.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

class FirstTriangle : public DemoApp<HWWindow>
{
public:
	virtual void OnCreate()
	{
		/* Normalized Device Coordinate(NDC)
		*
		* (-1.0,1.0)--------(1.0,1.0)
		*      |                |
		*      |                |
		*      |                |
		* (-1.0,-1.0)--------(1.0,-1.0)
		*/

		renderer_ = hw::GetRenderer();

		VertexStream streams[2] =
		{
			VertexStream(VertexStreamUsage::Position, 0, ElementType::F32, 3),
			VertexStream(VertexStreamUsage::Color, 0, ElementType::U32, 1)
		};
		VertexDeclarationRef vertexDecl = VertexDeclaration::Get(&streams[0], 2);
		vertexBuf_ = new VertexBuffer(vertexDecl, 3);
		vertexBuf_->SetStorageFlags(StorageFlags::Hardware);

		auto L = vertexBuf_->Lock(LockFlags::LockWrite);
		struct Vertex
		{
			Vector3f P;
			Color C;
		};
		Vertex v[3] = 
		{
			Vertex{ Vector3f(-1.0f, -1.0f, 0.0f), Color::c_white },
			Vertex{ Vector3f(0.0f, 1.0f, 0.0f),   Color::c_white },
			Vertex{ Vector3f(1.0f, -1.0f, 0.0f),  Color::c_white },
		};
		memcpy(L->GetData(), &v[0], sizeof(v));
		vertexBuf_->Unlock(L);
		vertexBuf_->Invalidate();
	}

	virtual void OnRender(const Timestep&)
	{
		if (renderer_)
		{
			// draw
			renderer_->BeginFrame(ClearFlags::Color | ClearFlags::Depth, Color(0xff000000), 1.0f, 0);

			renderer_->DrawPrimitive(PrimitiveType::Triangles, *(vertexBuf_->HWResource()), 1);

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
	FirstTriangle app;
	app.Create(640, 480, "First Triangle");
	app.Loop();
	return 0;
}
