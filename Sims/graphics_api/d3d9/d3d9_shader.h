/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   10:28
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_shader.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_shader
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Shader
*********************************************************************/
#ifndef __SHADER_DX_H__
#define __SHADER_DX_H__

#include "graphics/shader.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9Shader : public Shader
		{
		public:
			D3D9Shader();
			~D3D9Shader();

			void* GetShaderObj() const { return so_; }

			virtual bool IsValid() const;
			virtual bool Compile(ShaderDomain::Type type, const string& source);
			virtual bool LoadBinary(ShaderDomain::Type type, uint8* byteCode, uint32 byteCodeLength);
			virtual void Delete();
			virtual void Use();

			virtual UniformLoc GetUniformLoc(const char* name, UniformLoc parent = nullptr);
		private:
			bool CreateShaderObj(ShaderDomain::Type type, uint8* byteCode, uint32);

			void* so_; // shader object;
			ID3DXConstantTable* table_;
		};
	}
}

#endif
