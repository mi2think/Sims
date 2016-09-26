/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   14:07
	filename: 	D:\Code\Sims\Sims\core\texture.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	texture
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Texture
*********************************************************************/
#include "texture.h"
#include "image.h"
#include "graphics_api/renderer_rhi.h"

namespace sims
{
	Texture::Texture()
		: width_(0)
		, height_(0)
		, format_(PF_Unknown)
		, mipmapCount_(0)
		, filterMin_(TF_Linear)
		, filterMag_(TF_Linear)
		, filterMip_(TF_Linear)
		, wrapS_(TW_Clamp)
		, wrapT_(TW_Clamp)
		, renderID_(0)
		, storageFlags_(SF_Local)
	{
	}

	Texture::Texture(uint32 width, uint32 height, PixelFormat format, uint32 storgeFlags)
		: width_(width)
		, height_(height)
		, format_(format)
		, filterMin_(TF_Linear)
		, filterMag_(TF_Linear)
		, filterMip_(TF_Linear)
		, wrapS_(TW_Clamp)
		, wrapT_(TW_Clamp)
		, mipmapCount_(1)
		, renderID_(0)
		, storageFlags_(storgeFlags)
	{
		ImageRef image(new Image(width, height, format));
		mipmaps_.push_back(image);
	}

	Texture::Texture(const string& path, PixelFormat format, uint32 storgeFlags)
		: filterMin_(TF_Linear)
		, filterMag_(TF_Linear)
		, filterMip_(TF_Linear)
		, wrapS_(TW_Clamp)
		, wrapT_(TW_Clamp)
		, renderID_(0)
		, storageFlags_(storgeFlags)
	{
		Load(path, format);
	}

	Texture::Texture(const ImageRef& image, uint32 storgeFlags)
		: storageFlags_(storgeFlags)
	{
		SetImage(image);
	}

	Texture::~Texture()
	{
		Clear();
		
		if (!renderID_)
		{
			RHIRenderer::GetRenderer()->DeleteTexture(renderID_);
		}
	}

	void Texture::SetImage(const ImageRef& image)
	{
		width_ = image->GetWidth();
		height_ = image->GetHeight();
		format_ = image->GetFormat();
		mipmapCount_ = 1;
		filterMin_ = TF_Linear;
		filterMag_ = TF_Linear;
		filterMip_ = TF_Linear;
		wrapS_ = TW_Clamp;
		wrapT_ = TW_Clamp;
		mipmaps_.push_back(image);
	}

	void Texture::Load(const string& path, PixelFormat format)
	{
		Clear();

		ImageRef image = Image::FromFile(path, format);
		uint32 width = image->GetWidth();
		uint32 height = image->GetHeight();
		ASSERT(image->Valid());

		width_ = width;
		height_ = height;
		format_ = image->GetFormat();
		mipmaps_.push_back(image);
		mipmapCount_ = mipmaps_.size();
	}

	void Texture::SaveTGA(const string& path, uint32 level)
	{
		ASSERT(level < mipmaps_.size());
		mipmaps_[level]->SaveTGA(path);
	}

	void Texture::SavePNG(const string& path, uint32 level, bool filpped)
	{
		ASSERT(level < mipmaps_.size());
		mipmaps_[level]->SavePNG(path, filpped);
	}

	ImageRef Texture::GetImage(uint32 level) const
	{
		ASSERT(level >= 0 && level < mipmapCount_);
		return mipmaps_[level];
	}

	void Texture::GenMipmaps(ImageRef image)
	{
		// TODO
	}

	void Texture::Clear()
	{
		mipmaps_.clear();
		mipmapCount_ = 0;
	}

	void Texture::Invalidate()
	{
		if ((storageFlags_ & SF_Hardware) == 0)
			return;

		// collect invalid region, for update later
		vector<Recti> regions;
		Recti bbox;
		for (uint32 i = 0; i < mipmapCount_; ++i)
		{
			ImageRef image = GetImage(i);
			if (image)
			{
				regions.push_back(image->GetInvalidRegion());
				bbox |= regions.back();
				image->Validate(); // clear invalidate
			}
			else
				regions.push_back(Recti());
		}
		if (bbox.IsRectEmpty())
			return;

		// update texture
		RHIRenderer::GetRenderer()->UpdateTexture(*this, &regions[0]);
	}
}