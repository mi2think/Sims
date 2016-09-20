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
		Texture();
		Texture(uint32 width, uint32 height, PixelFormat format);
		Texture(const string& path, bool mipmaps = false);
		Texture(const ImageRef& image);
		~Texture();

		const string& GetName() const { return name_; }
		void SetImage(const ImageRef& image);
		void Load(const string& path, bool mipmaps = false);
		void SaveTGA(const string& path, uint32 level = 0);
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
		void SetWrapS(TextureWrap wrap) { wrapS_ = wrap; }
		void SetWrapT(TextureWrap wrap) { wrapT_ = wrap; }

		Color GetBorderColor() const { return borderColor_; }
		void SetBorderColor(const Color& c) { borderColor_ = c; }

		// hardware interface
		virtual void Bind(int) {}
		virtual void Unbind(int) {}
		virtual void Destory() {}
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
	};
}

#endif
