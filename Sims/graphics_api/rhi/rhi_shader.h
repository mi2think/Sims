/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:24
	filename: 	D:\Code\Sims\Sims\graphics_api\rhi\rhi_shader.h
	file path:	D:\Code\Sims\Sims\graphics_api\rhi
	file base:	rhi_shader
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Shader
*********************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include "graphics/graphics_fwd.h"

namespace sims
{
	namespace rhi
	{
		class RHIShader
		{
		public:
			RHIShader();
			virtual ~RHIShader();

			const string& GetSource() const { return source_; }
			ShaderDomain::Type GetType() const { return type_; }
			RenderID GetRenderID() const { id_; }

			bool IsValid() const { return id_ != 0; }

			virtual bool Compile(ShaderDomain::Type type, const string& source) = 0;
			virtual bool LoadBinary(ShaderDomain::Type type, uint8* byteCode, uint32 byteCodeLength) = 0;
			virtual void Delete() = 0;
			virtual void Use() = 0;

			bool CompileFromFile(ShaderDomain::Type type, const string& filename);
			bool LoadBinaryFromFile(ShaderDomain::Type type, const string& filename);
		protected:
			string source_;
			ShaderDomain::Type type_;
			RenderID id_;
		};
	}
}

#endif
