#include "sims.h"
#include "math/math.h"
#include "core/app.h"
#include "graphics/vertex_buffer.h"
#include "graphics/shader.h"
#include "graphics/program.h"
#include "graphics_api/sims_sdk_hw.h"
using namespace sims;

#if SIMS_SDK_IMPL_OGL
const char* VSSource = "#version 330\n"
"layout (location = 0) in vec3 Position;\n"
"layout (location = 1) in vec3 Color;\n"
"out vec3 VertexColor;"
"void main()"
"{"
"gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 1.0);"
"VertexColor = Color;"
"}";

const char* PSSource = "#version 330\n"
"in  vec3 VertexColor;"
"out vec4 FragColor;"
"void main()"
"{"
"FragColor = vec4(VertexColor, 1);"
"}";
#elif SIMS_SDK_IMPL_D3D9
const char* VSSource = "\n"
"struct VS_OUTPUT"
"{"
"float4 position : POSITION;"
"float3 color : COLOR;"
"};"
"VS_OUTPUT main(float3 pos : POSITION, float3 c : COLOR)"
"{"
"VS_OUTPUT o = (VS_OUTPUT)0;"
"o.position = float4(pos, 1) * float4(0.5, 0.5, 1, 1);"
"o.color = c;"
"return o;"
"}";

const char* PSSource = "\n"
"float4 main(float3 color : COLOR) : COLOR"
"{"
"return float4(color, 1);"
"}";
#endif

class Shaders : public App<HWWindow>
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
			Vertex{ Vector3f(-1.0f, -1.0f, 0.0f), Color::c_red.GetRGBVector3()   },
			Vertex{ Vector3f(0.0f, 1.0f, 0.0f),   Color::c_green.GetRGBVector3() },
			Vertex{ Vector3f(1.0f, -1.0f, 0.0f),  Color::c_blue.GetRGBVector3()  },
		};
		memcpy(L->GetData(), &v[0], sizeof(v));
		vertexBuf_->Unlock(L);
		vertexBuf_->Invalidate();
	}

	void SetupProg()
	{
		ShaderRef VS = new Shader(ShaderDomain::Vertex, VSSource, "main");
		ShaderRef PS = new Shader(ShaderDomain::Fragment, PSSource, "main");
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
	Shaders app;
	app.Create(640, 480, "Shaders");
	app.Loop();
	return 0;
}
