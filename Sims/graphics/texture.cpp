/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   14:07
	filename: 	D:\Code\Sims\Sims\graphics\texture.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	texture
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Texture
*********************************************************************/
#include "texture.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	Texture::Texture()
		: width_(0)
		, height_(0)
		, format_(PixelFormat::Unknown)
		, mipmapCount_(0)
		, storageFlags_(StorageFlags::Local)
	{
	}

	Texture::Texture(uint32 width, uint32 height, PixelFormat::Type format)
		: width_(width)
		, height_(height)
		, format_(format)
		, mipmapCount_(1)
		, storageFlags_(StorageFlags::Local)
	{
		ImageRef image(new Image(width, height, format));
		mipmaps_.push_back(image);
	}

	Texture::Texture(const string& path, PixelFormat::Type format)
		: storageFlags_(StorageFlags::Local)
	{
		Load(path, format);
	}

	Texture::Texture(const ImageRef& image)
		: storageFlags_(StorageFlags::Local)
	{
		SetImage(image);
	}

	Texture::~Texture()
	{
		Clear();
	}

	void Texture::SetImage(const ImageRef& image)
	{
		width_ = image->GetWidth();
		height_ = image->GetHeight();
		format_ = image->GetFormat();
		mipmapCount_ = 1;
		mipmaps_.push_back(image);
	}

	void Texture::Load(const string& path, PixelFormat::Type format)
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
		if ((storageFlags_ & StorageFlags::Hardware) == 0)
			return;

		// collect invalid region, for update later
		TBuffer<Recti> regions(mipmapCount_);
		Recti bbox;
		for (uint32 i = 0; i < mipmapCount_; ++i)
		{
			ImageRef image = GetImage(i);
			if (image)
			{
				regions[i] = image->GetInvalidRegion();
				bbox |= regions[i];
				image->Validate(); // clear invalidate
			}
			else
				regions[i] = Recti();
		}
		if (bbox.IsRectEmpty())
			return;

		// update texture
		if (!HWResource_)
			HWResource_ = hw::CreateResource<TextureResource>();

		HWResource_->Attach(this);
		HWResource_->SetUpdateRegions(regions);
		HWResource_->UpdateResource();
	}

	void Texture::SetSamplerStatus(const TextureSamplerStatus& status)
	{
		samplerStatus_ = status;
		
		if ((storageFlags_ & StorageFlags::Hardware) == 0)
			return;
		if (HWResource_)
			HWResource_->OnSamplerStatusUpdated();
	}
}