/********************************************************************
	created:	2016/11/06
	created:	6:11:2016   23:18
	filename: 	D:\Code\Sims\Sims\graphics\hw\shader.h
	file path:	D:\Code\Sims\Sims\graphics\hw
	file base:	shader
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Shader
*********************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include "graphics/graphics_fwd.h"
#include "render_resource.h"

namespace sims
{
	class Shader : public IResourceOperation
	{
	public:
		Shader();
		Shader(ShaderDomain::Type type);
		Shader(ShaderDomain::Type type, const char* source, const char* entryName);
		~Shader();

		void SetSource(const char* source);
		void SetSourceFromFile(const char* filename);
		void SetType(ShaderDomain::Type type);
		void SetEntryName(const char* entryName);

		const string& GetSource() const { return source_; }
		ShaderDomain::Type GetType() const { return type_; }
		const char* GetTypeDesc() const;
		const string& GetEntryName() const { return entryName_; }

		// ~ IResourceOperation
		virtual void Invalidate() override;
	private:
		virtual void Create();
		// ~ IResourceOperation

		string source_;
		string entryName_;
		ShaderDomain::Type type_;
		bool invalid_;
	};
}


#endif