/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   11:28
	filename: 	D:\Code\Sims\Sims\graphics\texture.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	texture
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Texture
*********************************************************************/
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "graphics_fwd.h"
#include "core/color.h"
#include "core/image.h"

namespace sims
{
	class Texture
	{
	public:
		Texture();
		Texture(uint32 width, uint32 height, PixelFormat::Type format, uint32 storgeFlags = StorageFlags::Local);
		Texture(const string& path, PixelFormat::Type format, uint32 storgeFlags = StorageFlags::Local);
		Texture(const ImageRef& image, uint32 storgeFlags = StorageFlags::Local);
		~Texture();

		const string& GetName() const { return name_; }
		void SetImage(const ImageRef& image);
		void Load(const string& path, PixelFormat::Type format);
		void SaveTGA(const string& path, uint32 level = 0);
		void SavePNG(const string& path, uint32 level = 0, bool filpped = false);
		void Clear();

		int GetWidth() const { return width_; }
		int GetHeight() const { return height_; }
		PixelFormat::Type GetFormat() const { return format_; }

		int GetMipmapCount() const { return mipmapCount_; }
		bool HasMips() const { return mipmapCount_ > 1; }
		ImageRef GetImage(uint32 level) const;

		TextureFilter::Type GetFilterMin() const { return filterMin_; }
		TextureFilter::Type GetFilterMag() const { return filterMag_; }
		TextureFilter::Type GetFilterMip() const { return filterMip_; }
		void SetFilterMin(TextureFilter::Type filter) { filterMin_ = filter; }
		void SetFilterMag(TextureFilter::Type filter) { filterMag_ = filter; }
		void SetFilterMip(TextureFilter::Type filter) { filterMip_ = filter; }

		TextureWrap::Type GetWrapS() const { return wrapS_; }
		TextureWrap::Type GetWrapT() const { return wrapT_; }
		Color GetBorderColor() const { return borderColor_; }
		void SetWrapS(TextureWrap::Type wrap) { wrapS_ = wrap; }
		void SetWrapT(TextureWrap::Type wrap) { wrapT_ = wrap; }
		void SetBorderColor(const Color& c) { borderColor_ = c; }

		RenderID GetRenderID() const { return renderID_; }
		void SetRenderID(RenderID id) { renderID_ = id; }

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }

		// propagates changes on the texture and its mipmap images to the renderer.
		//   you must call invalidate after modifying any texture paramters or image data,
		//   for it to be uploaded to GPU.
		void Invalidate();
	protected:
		void GenMipmaps(ImageRef image);

		string name_; // file name if load from file
		uint32 width_;
		uint32 height_;
		PixelFormat::Type format_;
		
		Color borderColor_;
		TextureFilter::Type filterMin_;
		TextureFilter::Type filterMag_;
		TextureFilter::Type filterMip_;
		TextureWrap::Type wrapS_;
		TextureWrap::Type wrapT_;

		uint32 mipmapCount_;
		vector<ImageRef> mipmaps_;

		uint32 storageFlags_;
		RenderID renderID_;
	};
}

#endif
