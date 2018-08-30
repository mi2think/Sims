/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   14:07
	filename: 	D:\Code\Sims\Sims\graphics\hw\texture.cpp
	file path:	D:\Code\Sims\Sims\graphics\hw
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
	{
	}

	Texture::Texture(uint32 width, uint32 height, PixelFormat::Type format)
		: width_(width)
		, height_(height)
		, format_(format)
		, mipmapCount_(1)
	{
		ImageRef image(new Image(width, height, format));
		mipmaps_.push_back(image);
	}

	Texture::Texture(const string& path, PixelFormat::Type format)
	{
		Load(path, format);
	}

	Texture::Texture(const ImageRef& image)
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
		// collect invalid region, for update later
		regions_.resize(mipmapCount_);
		Recti bbox;
		for (uint32 i = 0; i < mipmapCount_; ++i)
		{
			ImageRef image = GetImage(i);
			if (image)
			{
				regions_[i] = image->GetInvalidRegion();
				bbox |= regions_[i];
				image->Validate(); // clear invalidate
			}
			else
				regions_[i] = Recti();
		}
		if (bbox.IsEmpty())
			return;

		IResourceOperation::Invalidate();
	}

	void Texture::Create()
	{
		if (HWResource_)
		{
			Release();
		}

		HWResource_ = hw::CreateResource<TextureResource>();
	}

	void Texture::PreUpdate()
	{
		ASSERT(HWResource_);
		HWResource_->OnOperate(RenderResource::OP_TEX_UPDATE_REGIONS, &regions_);
	}

	void Texture::SetSamplerStatus(const TextureSamplerStatus& status)
	{
		samplerStatus_ = status;
		
		if (HWResource_)
			HWResource_->OnOperate(RenderResource::OP_TEX_UPDATE_SAMPLE);
	}
}