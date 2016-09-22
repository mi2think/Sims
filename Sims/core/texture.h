/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   11:28
	filename: 	D:\Code\Sims\Sims\core\texture.h
	file path:	D:\Code\Sims\Sims\core
	file base:	texture
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Texture
*********************************************************************/
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "sims.h"
#include "color.h"

namespace sims
{
	class Texture
	{
	public:
		enum StorageFlags
		{
			SF_Local		= BIT(0),
			SF_Hardware		= BIT(1),
			// hint for hardware, only valid if SF_Hardware set
			SF_HintDynamic	= BIT(2)
		};

		Texture();
		Texture(uint32 width, uint32 height, PixelFormat format, uint32 storgeFlags = SF_Local);
		Texture(const string& path, PixelFormat format, uint32 storgeFlags = SF_Local);
		Texture(const ImageRef& image, uint32 storgeFlags = SF_Local);
		~Texture();

		const string& GetName() const { return name_; }
		void SetImage(const ImageRef& image);
		void Load(const string& path, PixelFormat format);
		void SaveTGA(const string& path, uint32 level = 0);
		void SavePNG(const string& path, uint32 level = 0, bool filpped = false);
		void Clear();

		int GetWidth() const { return width_; }
		int GetHeight() const { return height_; }
		PixelFormat GetFormat() const { return format_; }

		int GetMipmapCount() const { return mipmapCount_; }
		bool HasMips() const { return mipmapCount_ > 1; }
		ImageRef GetImage(uint32 level) const;

		TextureFilter GetFilterMin() const { return filterMin_; }
		TextureFilter GetFilterMag() const { return filterMag_; }
		TextureFilter GetFilterMip() const { return filterMip_; }
		void SetFilterMin(TextureFilter filter) { filterMin_ = filter; }
		void SetFilterMag(TextureFilter filter) { filterMag_ = filter; }
		void SetFilterMip(TextureFilter filter) { filterMip_ = filter; }

		TextureWrap GetWrapS() const { return wrapS_; }
		TextureWrap GetWrapT() const { return wrapT_; }
		Color GetBorderColor() const { return borderColor_; }
		void SetWrapS(TextureWrap wrap) { wrapS_ = wrap; }
		void SetWrapT(TextureWrap wrap) { wrapT_ = wrap; }
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
		PixelFormat format_;
		
		Color borderColor_;
		TextureFilter filterMin_;
		TextureFilter filterMag_;
		TextureFilter filterMip_;
		TextureWrap wrapS_;
		TextureWrap wrapT_;

		uint32 mipmapCount_;
		vector<ImageRef> mipmaps_;

		uint32 storageFlags_;
		RenderID renderID_;
	};
}

#endif
