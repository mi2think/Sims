/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   11:28
	filename: 	D:\Code\Sims\Sims\graphics\hw\texture.h
	file path:	D:\Code\Sims\Sims\graphics\hw
	file base:	texture
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Texture
*********************************************************************/
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "graphics/graphics_fwd.h"
#include "texture_sampler_status.h"
#include "core/image.h"
#include "render_resource.h"

namespace sims
{
	class Texture : public IResourceOperation
	{
	public:
		Texture();
		Texture(uint32 width, uint32 height, PixelFormat::Type format);
		Texture(const string& path, PixelFormat::Type format);
		Texture(const ImageRef& image);
		~Texture();

		const string& GetName() const { return name_; }
		void Load(const string& path, PixelFormat::Type format);
		void SaveTGA(const string& path, uint32 level = 0);
		void SavePNG(const string& path, uint32 level = 0, bool filpped = false);
		void Clear();

		ImageRef GetImage(uint32 level) const;
		uint32 GetWidth() const { return width_; }
		uint32 GetHeight() const { return height_; }
		PixelFormat::Type GetFormat() const { return format_; }

		uint32 GetMipmapCount() const { return mipmapCount_; }
		bool HasMips() const { return mipmapCount_ > 1; }
		void SetImage(const ImageRef& image);

		void SetSamplerStatus(const TextureSamplerStatus& status);
		const TextureSamplerStatus& GetSamplerStatus() const { return samplerStatus_; }
		
		// ~ IResourceOperation
		virtual void Invalidate() override;
	private:
		virtual void Create();
		virtual void PreUpdate();
		// ~ IResourceOperation

		void GenMipmaps(ImageRef image);

		string name_; // file name if load from file
		uint32 width_;
		uint32 height_;
		PixelFormat::Type format_;

		uint32 mipmapCount_;
		vector<ImageRef> mipmaps_;
		vector<Recti> regions_;

		uint32 storageFlags_;

		// texture sampler paramters
		// no need to upload to GPU if changes, it will bind when use
		TextureSamplerStatus samplerStatus_;
	};
}

#endif
