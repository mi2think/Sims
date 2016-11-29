/********************************************************************
	created:	2016/09/07
	created:	7:9:2016   21:51
	filename: 	D:\Code\Sims\Sims\image.cpp
	file path:	D:\Code\Sims\Sims
	file base:	image
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Image
*********************************************************************/
#include "image.h"
#include "Color.h"
#include "vfs.h"

#include "stb_image.h"
#include "stb_image_write.h"

namespace sims
{
	LockedImage::LockedImage()
	{
		Clear();
	}

	LockedImage::LockedImage(Image* image, uint32 lockFlags)
	{
		Init(image, lockFlags);
	}

	void LockedImage::Init(Image* image, uint32 lockFlags)
	{
		image_ = image;
		lockFlags_ = lockFlags;
	}

	void LockedImage::Clear()
	{
		image_ = nullptr;
		lockFlags_ = 0;
	}

	char* LockedImage::GetData()
	{
		return image_->GetData();
	}

	const char* LockedImage::GetData() const
	{
		return image_->GetData();
	}

	uint32 LockedImage::GetImageDataSize() const
	{
		return image_->GetImageDataSize();
	}

	uint32 LockedImage::GetLockFlags() const
	{
		return lockFlags_;
	}

	Image::Image()
		: width_(0)
		, height_(0)
		, format_(PixelFormat::Unknown)
		, bytesPerPixel_(0)
		, lockedCount_(0)
	{
	}

	Image::Image(uint32 width, uint32 height, PixelFormat::Type format)
		: width_(width)
		, height_(height)
		, format_(format)
		, lockedCount_(0)
		, invalidRegion_(0, 0, width, height)
	{
		bytesPerPixel_ = Image::GetBytesPerPixel(format_);
		uint32 dataSize = width_ * height_ * bytesPerPixel_;
		data_.Resize(dataSize);
	}

	Image::Image(const void* data, uint32 length, PixelFormat::Type format)
	{
		int w = 0;
		int h = 0;
		int comp = 0;
		int result = stbi_info_from_memory((const stbi_uc*)data, length, &w, &h, &comp);
		ASSERT(result && "error: stbi not suppot it");

		if (! result)
		{
			width_ = w;
			height_ = h;
			format_ = format;
			data_.Resize(length);
			memcpy(data_.GetData(), data, length);
			bytesPerPixel_ = Image::GetBytesPerPixel(format_);
			lockedCount_ = 0;
		}
		invalidRegion_.SetRect(0, 0, w, h);
	}

	Image::~Image()
	{
	}

	void Image::SaveTGA(const string& path)
	{
		// convert to PixelFormat::R8G8B8A8
		ImageRef origin(this);
		origin.IncRef();
		ImageRef image = ToPixelFormat(origin, PixelFormat::R8G8B8A8);

		// write tga
		IOutputStreamRef stream = VFS::GetVFS().OpenOutputStream(path);
		if (!stream)
		{
			LOG_ERROR("open file '%s' for write failed!", path.c_str());
			return;
		}

		struct TGAHeader
		{
			uint8 idlength;
			uint8 colourmaptype;
			uint8 datatypecode;
			uint16 colourmapOrigin;
			uint16 colourmapLength;
			uint8 colourmapDepth;
			uint16 x_origin;
			uint16 y_origin;
			uint16 width;
			uint16 height;
			uint8 bitsperpixel;
			uint8 imagedescriptor;
		};
		TGAHeader header;

		memset(&header, 0, sizeof(TGAHeader));
		header.datatypecode = 2;	// uncompressed RGB
		header.width = (uint16)width_;
		header.height = (uint16)height_;
		header.bitsperpixel = 32;

		// write out the TGA header
		stream->Write((char*)&header.idlength, 1);
		stream->Write((char*)&header.colourmaptype, 1);
		stream->Write((char*)&header.datatypecode, 1);
		stream->Write((char*)&header.colourmapOrigin, 2);
		stream->Write((char*)&header.colourmapLength, 2);
		stream->Write((char*)&header.colourmapDepth, 1);
		stream->Write((char*)&header.x_origin, 2);
		stream->Write((char*)&header.y_origin, 2);
		stream->Write((char*)&header.width, 2);
		stream->Write((char*)&header.height, 2);
		stream->Write((char*)&header.bitsperpixel, 1);
		stream->Write((char*)&header.imagedescriptor, 1);

		auto L = image->Lock(LockRead);
		const char* src = L->GetData();
		for (int y = height_ - 1; y >= 0; --y)
		{
			const char* row = src + y * width_ * 4;
			for (int x = 0; x < (int)width_ * 4; x += 4)
			{
				char r = row[x];
				char g = row[x + 1];
				char b = row[x + 2];
				char a = row[x + 3];
				stream->Write(&b, 1);
				stream->Write(&g, 1);
				stream->Write(&r, 1);
				stream->Write(&a, 1);
			}
		}
		image->Unlock(L);
	}

	void Image::SavePNG(const string& path, bool filpped)
	{
		ASSERT(format_ == PixelFormat::A8 || format_ == PixelFormat::R8G8B8 || format_ == PixelFormat::R8G8B8A8);

		IOutputStreamRef stream = VFS::GetVFS().OpenOutputStream(path);
		if (!stream)
		{
			LOG_ERROR("open file '%s' for write failed!", path.c_str());
			return;
		}

		int len = 0;
		auto L = Lock(LockRead);
		const char* buffer = nullptr;
		if (filpped)
		{
			int pitch = width_ * bytesPerPixel_;
			buffer = (const char*)stbi_write_png_to_mem((unsigned char*)L->GetData() + (height_ - 1) * pitch, 
				-pitch, 
				width_, 
				height_, 
				bytesPerPixel_,
				&len);
		}
		else
		{
			buffer = (const char*)stbi_write_png_to_mem((unsigned char*)L->GetData(),
				width_ * bytesPerPixel_,
				width_,
				height_,
				bytesPerPixel_,
				&len);
		}

		ASSERT(buffer && "can not save to png");
		Unlock(L);
		stream->Write(buffer, len);
		stbi_image_free((void*)buffer);
	}

	uint32 Image::GetBytesPerPixel(PixelFormat::Type format)
	{
		switch (format)
		{
		case PixelFormat::A8:
			return 1;
		case PixelFormat::R8G8B8:
			return 3;
		case PixelFormat::R8G8B8A8:
		case PixelFormat::A8R8G8B8:
			return 4;
		case PixelFormat::FloatRGB:
			return 12;
		case PixelFormat::FloatRGBA:
			return 16;
		default:
			ASSERT(0 && "error: unsupported image format");
			break;
		}
		return 0;
	}

	ImageRef Image::FromFile(const string& path, PixelFormat::Type format)
	{
		IInputStreamRef stream = VFS::GetVFS().OpenInputStream(path);
		Buffer buffer = stream->Read();

		ImageRef image;

		stbi_uc* stbi_data = nullptr;
		int width = 0;
		int height = 0;
		int comp = 0;
		
		uint32 bytesPerPixel = GetBytesPerPixel(format);
		if (bytesPerPixel == 1)
		{
			// load gray(may be use as alpha)
			stbi_data = stbi_load_from_memory((const stbi_uc*)buffer.GetData(), buffer.GetSize(), &width, &height, &comp, 1);
			ASSERT(comp == 1);
			if (stbi_data != nullptr)
			{
				ImageRef origin(new Image(width, height, format));
				void* data = origin->GetData();
				memcpy(data, stbi_data, origin->GetBytesPerPixel() * width * height);
				image = origin;
			}
		}
		else
		{
			// load PixelFormat::R8G8B8A8 data, then convert if need
			stbi_data = stbi_load_from_memory((const stbi_uc*)buffer.GetData(), buffer.GetSize(), &width, &height, &comp, 4);
			if (stbi_data != nullptr)
			{
				ImageRef origin(new Image(width, height, PixelFormat::R8G8B8A8));
				void* data = origin->GetData();
				memcpy(data, stbi_data, origin->GetBytesPerPixel() * width * height);
				image = ToPixelFormat(origin, format);
			}
		}

		if (stbi_data)
			stbi_image_free(stbi_data);
		else
			LOG_ERROR("error: stbi decode image %s filed!\n", path.c_str());

		return image;
	}

	ImageRef Image::ToPixelFormat(const ImageRef& origin, PixelFormat::Type format)
	{
		ASSERT(origin != nullptr);
		ASSERT(origin->GetFormat() == PixelFormat::R8G8B8A8);

		PixelFormat::Type pf = origin->GetFormat();
		int w = origin->GetWidth();
		int h = origin->GetHeight();
		auto L = origin->Lock(LockRead);
		const char* data = L->GetData();

		// same format
		if (format == pf)
		{
			origin->Unlock(L);
			return origin;
		}

		ImageRef image = new Image(w, h, format);
		if (format == PixelFormat::FloatRGBA)
		{
			const char* src = data;
			float* dest = (float*)image->GetData();
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					dest[0] = src[0] / 255.0f;
					dest[1] = src[1] / 255.0f;
					dest[2] = src[2] / 255.0f;
					dest[3] = src[3] / 255.0f;
					dest += 4;
					src += 4;
				}
			}
		}
		else if (format == PixelFormat::FloatRGB)
		{
			const char* src = data;
			float* dest = (float*)image->GetData();
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					dest[0] = src[0] / 255.0f;
					dest[1] = src[1] / 255.0f;
					dest[2] = src[2] / 255.0f;
					dest += 3;
					src += 4;
				}
			}
		}
		else if (format == PixelFormat::R8G8B8)
		{
			const char* src = data;
			char* dest = image->GetData();
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					dest[0] = src[0];
					dest[1] = src[1];
					dest[2] = src[2];
					dest += 3;
					src += 4;
				}
			}
		}
		else if (format == PixelFormat::A8R8G8B8)
		{
			const char* src = data;
			char* dest = image->GetData();
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					*((Color*)dest) = Color(src[0], src[1], src[2], src[3]).GetPackedARGB();
					dest += 4;
					src += 4;
				}
			}
		}
		origin->Unlock(L);
		return image;
	}

	void Image::Invalidate()
	{
		InvalidateRegion(Recti(0, 0, GetWidth(), GetHeight()));
	}

	void Image::InvalidateRegion(const Recti& region)
	{
		invalidRegion_ |= region;
	}

	void Image::Validate()
	{
		invalidRegion_.Width(0);
		invalidRegion_.Height(0);
	}

	LockedImage* Image::Lock(uint32 lockFlags)
	{
		if (lockedCount_ > 0)
		{
			// read is ok if pre-lock is read
			if ((lockFlags & LockWrite) != 0 ||
				(lockedImage_.lockFlags_ & LockWrite) != 0)
			{
				ASSERT(false && "lock locked image");
				return nullptr;
			}
		}

		if (!Valid())
		{
			ASSERT(false && "image has no data");
			return nullptr;
		}
		++lockedCount_;
		lockedImage_.Init(this, lockFlags);
		return &lockedImage_;
	}

	void Image::Unlock(LockedImage* L)
	{
		if (!L || L->image_ != this)
		{
			ASSERT(false && "can not unlock image");
			return;
		}

		--lockedCount_;
		if (lockedCount_ == 0)
			L->Clear();
		else
			ASSERT(L->GetLockFlags() == LockRead && "multi-lock must be read");
	}
}