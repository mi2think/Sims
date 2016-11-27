/********************************************************************
	created:	2016/11/08
	created:	8:11:2016   14:29
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_program.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_program
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Program
*********************************************************************/
#include "d3d9_program.h"
#include "d3d9_shader.h"

namespace sims
{
	namespace d3d9
	{
		D3D9Program::D3D9Program()
		{
		}

		D3D9Program::~D3D9Program()
		{
			DeleteProgramInternal();
		}

		void D3D9Program::BindProgram()
		{
			for (auto& so : shaders_)
			{
				if (so)
					so->Bind();
				else
				{
					switch (so->GetType())
					{
					case ShaderDomain::Vertex:
						g_pD3DD->SetVertexShader(nullptr);
						break;
					case ShaderDomain::Fragment:
						g_pD3DD->SetPixelShader(nullptr);
						break;
					default:
						break;
					}
				}
			}
		}

		void D3D9Program::DeleteProgram()
		{
			DeleteProgramInternal();
		}

		void D3D9Program::DeleteProgramInternal()
		{
			for (auto& so : shaders_)
			{
				so = nullptr;
			}
		}

		UniformLoc D3D9Program::VSGetUniformLoc(const char* name, UniformLoc parent) const
		{
			return shaders_[ShaderDomain::Vertex] ? shaders_[ShaderDomain::Vertex]->GetUniformLoc(name, parent) : 0;
		}

		UniformLoc D3D9Program::FSGetUniformLoc(const char* name, UniformLoc parent) const
		{
			return shaders_[ShaderDomain::Fragment] ? shaders_[ShaderDomain::Fragment]->GetUniformLoc(name, parent) : 0;
		}

		void D3D9Program::VSBindUniform(UniformLoc loc, const void* data, uint32 count, UniformDataType::Type type)
		{
			auto d3d9Shader = shaders_[ShaderDomain::Vertex].As<D3D9Shader>();
			ASSERT(d3d9Shader);
			BindUniform(d3d9Shader->GetConstTable(), loc, data, count, type);
		}

		void D3D9Program::FSBindUniform(UniformLoc loc, const void* data, uint32 count, UniformDataType::Type type)
		{
			auto d3d9Shader = shaders_[ShaderDomain::Fragment].As<D3D9Shader>();
			ASSERT(d3d9Shader);
			BindUniform(d3d9Shader->GetConstTable(), loc, data, count, type);
		}

		void D3D9Program::BindUniform(ID3DXConstantTable* table, UniformLoc loc, const void* data, uint32 count, UniformDataType::Type type)
		{
			D3DXHANDLE location = (D3DXHANDLE)loc;
			switch (type)
			{
			case UniformDataType::Int:
				table->SetIntArray(g_pD3DD, location, (const int*)data, count);
				break;
			case UniformDataType::Int2:
				table->SetIntArray(g_pD3DD, location, (const int*)data, count * 2);
				break;
			case UniformDataType::Int3:
				table->SetIntArray(g_pD3DD, location, (const int*)data, count * 3);
				break;
			case UniformDataType::Int4:
				table->SetIntArray(g_pD3DD, location, (const int*)data, count * 4);
				break;
			case UniformDataType::F32:
				table->SetFloatArray(g_pD3DD, location, (const float*)data, count);
				break;
			case UniformDataType::Vec2:
				table->SetFloatArray(g_pD3DD, location, (const float*)data, count * 2);
				break;
			case UniformDataType::Vec3:
				table->SetFloatArray(g_pD3DD, location, (const float*)data, count * 3);
				break;
			case UniformDataType::Vec4:
				table->SetVectorArray(g_pD3DD, location, (const D3DXVECTOR4*)data, count);
				break;
			case UniformDataType::Mat4:
				table->SetMatrixArray(g_pD3DD, location, (const D3DXMATRIX*)data, count);
				break;
			case UniformDataType::Color:
				{
					ASSERT(count == 1);
					const Color& c = *(const Color*)data;
					auto v = c.GetRGBAVector4();
					table->SetVector(g_pD3DD, location, (const D3DXVECTOR4*)&v);
				}
				break;
			case UniformDataType::Sampler2D:
			case UniformDataType::Mat3:
			case UniformDataType::Unknown:
			default:
				ASSERT(false && "Unspported uniform type");
				break;
			}
		}
	}
}