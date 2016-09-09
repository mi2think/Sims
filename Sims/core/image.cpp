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
#include "input_stream.h"
#include "output_stream.h"
#include "file_system.h"
#include "platform/platform.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

namespace sims
{
	Image::Image()
		: width_(0)
		, height_(0)
		, format_(PF_Unknown)
		, bytesPerPixel_(0)
		, dataSize_(0)
		, data_(nullptr)
	{
	}

	Image::Image(uint32 width, uint32 height, PixelFormat format)
		: width_(width)
		, height_(height)
		, format_(format)
	{
		bytesPerPixel_ = Image::GetBytesPerPixel(format_);
		dataSize_ = width_ * height_ * bytesPerPixel_;
		data_ = new uint8[dataSize_];
	}

	Image::Image(const void* data, uint32 length, PixelFormat format)
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
			dataSize_ = length;
			format_ = format;
			data_ = new uint8[dataSize_];
			memcpy(data_, data, dataSize_);
			bytesPerPixel_ = Image::GetBytesPerPixel(format_);
		}
	}

	Image::~Image()
	{
		SAFE_DELETEARRAY(data_);
	}

	void Image::SaveTGA(const string& path)
	{
		// convert to PF_R8G8B8A8
		ImageRef origin(this);
		origin.IncRef();
		ImageRef image = ToPixelFormat(origin, PF_R8G8B8A8);

		// write tga
		IOutputStreamRef stream = Platform::GetFileSystem()->OpenOutputStream(path);
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
		stream->Write((uint8*)&header.idlength, 1);
		stream->Write((uint8*)&header.colourmaptype, 1);
		stream->Write((uint8*)&header.datatypecode, 1);
		stream->Write((uint8*)&header.colourmapOrigin, 2);
		stream->Write((uint8*)&header.colourmapLength, 2);
		stream->Write((uint8*)&header.colourmapDepth, 1);
		stream->Write((uint8*)&header.x_origin, 2);
		stream->Write((uint8*)&header.y_origin, 2);
		stream->Write((uint8*)&header.width, 2);
		stream->Write((uint8*)&header.height, 2);
		stream->Write((uint8*)&header.bitsperpixel, 1);
		stream->Write((uint8*)&header.imagedescriptor, 1);

		const uint8* src = image->GetData();
		for (uint32 y = height_ - 1; y >= 0; --y)
		{
			const uint8* row = src + y * width_ * 4;
			for (uint32 x = 0; x < width_ * 4; x += 4)
			{
				uint8 r = row[x];
				uint8 g = row[x + 1];
				uint8 b = row[x + 2];
				uint8 a = row[x + 3];
				stream->Write(&b, 1);
				stream->Write(&g, 1);
				stream->Write(&r, 1);
				stream->Write(&a, 1);
			}
		}
	}

	uint32 Image::GetBytesPerPixel(PixelFormat format)
	{
		switch (format)
		{
		case PF_R8G8B8:
			return 3;
			break;
		case PF_R8G8B8A8:
		case PF_A8R8G8B8:
			return 4;
			break;
		case PF_FloatRGB:
			return 12;
		case PF_FloatRGBA:
			return 16;
			break;
		default:
			ASSERT(0 && "error: unsupported image format");
			break;
		}
		return 0;
	}

	ImageRef Image::FromFile(const string& path, PixelFormat format)
	{
		IInputStreamRef stream = Platform::GetFileSystem()->OpenInputStream(path);
		vector<uint8> buffer = stream->Read();

		// load PF_R8G8B8A8 data
		int width = 0;
		int height = 0;
		int comp = 0;
		stbi_uc* stbi_data = stbi_load_from_memory(&buffer[0], buffer.size(), &width, &height, &comp, 4);
		if (!stbi_data)
		{
			LOG_ERROR("error: stbi decode image %s filed!\n", path.c_str());
			return ImageRef();
		}

		ImageRef origin(new Image(width, height, PF_R8G8B8A8));
		void* data = origin->GetData();
		memcpy(data, stbi_data, origin->GetBytesPerPixel() * width * height);
		ImageRef image = ToPixelFormat(origin, format);

		stbi_image_free(stbi_data);

		return image;
	}

	ImageRef Image::ToPixelFormat(const ImageRef& origin, PixelFormat format)
	{
		ASSERT(origin != nullptr);
		ASSERT(origin->GetFormat() == PF_R8G8B8A8);

		PixelFormat pf = origin->GetFormat();
		int w = origin->GetWidth();
		int h = origin->GetHeight();
		const uint8* data = origin->GetData();

		// same format
		if (format == pf)
		{
			return origin;
		}

		ImageRef image = new Image(w, h, format);
		if (format == PF_FloatRGBA)
		{
			const uint8* src = (const uint8*)data;
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
		else if (format == PF_FloatRGB)
		{
			const uint8* src = (const uint8*)data;
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
		else if (format == PF_R8G8B8)
		{
			const uint8* src = (const uint8*)data;
			uint8* dest = image->GetData();
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
		else if (format == PF_A8R8G8B8)
		{
			const uint8* src = (const uint8*)data;
			uint8* dest = image->GetData();
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					*((Color*)dest[0]) = Color(src[0], src[1], src[2], src[3]).ToPackedARGB();
					dest += 4;
					src += 4;
				}
			}
		}

		return image;
	}
}