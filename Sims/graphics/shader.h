/********************************************************************
	created:	2016/11/06
	created:	6:11:2016   23:18
	filename: 	D:\Code\Sims\Sims\graphics\shader.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	shader
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Shader
*********************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include "graphics_fwd.h"

namespace sims
{
	class Shader
	{
	public:
		Shader();
		virtual ~Shader();

		const string& GetSource() const { return source_; }
		ShaderDomain::Type GetType() const { return type_; }

		bool CompileFromFile(ShaderDomain::Type type, const string& filename);
		bool LoadBinaryFromFile(ShaderDomain::Type type, const string& filename);

		virtual bool IsValid() const = 0;
		virtual bool Compile(ShaderDomain::Type type, const string& source) = 0;
		virtual bool LoadBinary(ShaderDomain::Type type, uint8* byteCode, uint32 byteCodeLength) = 0;
		virtual void Delete() = 0;
		virtual void Bind() = 0;

		virtual UniformLoc GetUniformLoc(const char* name, UniformLoc parent = nullptr) = 0;
	protected:
		string source_;
		ShaderDomain::Type type_;
	};
}


#endif