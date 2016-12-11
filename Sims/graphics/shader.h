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
#include "render_resource.h"

namespace sims
{
	class Shader
	{
	public:
		Shader();
		Shader(ShaderDomain::Type type);
		~Shader();

		void SetSource(const char* source);
		void SetSourceFromFile(const char* filename);
		void SetType(ShaderDomain::Type type);
		void SetEntryName(const char* entryName);

		const Buffer& GetSource() const { return source_; }
		ShaderDomain::Type GetType() const { return type_; }
		const Buffer& GetEntryName() const { return entryName_; }

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }

		// propagates changes on the shader buffer to the renderer.
		//   you must call invalidate after modifying vertex buffer data,
		//   for it to be uploaded to GPU.
		void Invalidate();

		// Hardware resource
		ShaderResourceRef& HWResource() { return HWResource_; }
	private:
		Buffer source_;
		Buffer entryName_;
		ShaderDomain::Type type_;
		uint32 storageFlags_;

		ShaderResourceRef HWResource_;
	};
}


#endif