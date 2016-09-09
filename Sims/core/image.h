/********************************************************************
	created:	2016/09/07
	created:	7:9:2016   21:28
	filename: 	D:\Code\Sims\Sims\image.h
	file path:	D:\Code\Sims\Sims
	file base:	image
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Image
*********************************************************************/
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "sims.h"
#include "pixel_format.h"

namespace sims
{
	class Image : public NonCopyable
	{
	public:
		Image();
		Image(uint32 width, uint32 height, PixelFormat format);
		Image(const void* data, uint32 length, PixelFormat format);
		~Image();

		bool Valid() const { return data_ != nullptr; }
		uint32 GetWidth() const { return width_; }
		uint32 GetHeight() const { return height_; }
		PixelFormat GetFormat() const { return format_; }

		int GetBytesPerPixel() const { return bytesPerPixel_; }
		int GetImageDataSize() const { return dataSize_; }

		uint8* GetData() { return data_; }
		const uint8* GetData() const { return data_; }

		void SaveTGA(const string& path);

		static uint32 GetBytesPerPixel(PixelFormat format);
		static ImageRef FromFile(const string& path, PixelFormat format = PF_R8G8B8A8);
		static ImageRef ToPixelFormat(const ImageRef& origin, PixelFormat format);
	private:
		uint32 width_;
		uint32 height_;
		PixelFormat format_;
		uint32 bytesPerPixel_;
		uint32 dataSize_;
		uint8* data_;
	};
}

#endif
