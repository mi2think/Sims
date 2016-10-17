/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:24
	filename: 	D:\Code\Sims\Sims\graphics_api\rhi\shader_rhi.h
	file path:	D:\Code\Sims\Sims\graphics_api\rhi
	file base:	shader_rhi
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Shader
*********************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include "sims.h"

namespace sims
{
	class RHIShader
	{
	public:
		RHIShader();
		RHIShader(ShaderType type, const string& source);
		virtual ~RHIShader();

		const string& GetSource() const { return source_; }
		ShaderType GetType() const { return type_; }
		RenderID GetRenderID() const { id_; }

		bool IsValid() const { return id_ != 0; }

		virtual bool Compile(ShaderType type, const string& source) = 0;
	protected:
		string source_;
		ShaderType type_;
		RenderID id_;
	};
}

#endif