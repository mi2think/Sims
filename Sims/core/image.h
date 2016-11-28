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

#include "graphics/graphics_fwd.h"
#include "math/rectangle.h"

namespace sims
{
	class LockedImage
	{
	public:
		LockedImage();
		LockedImage(Image* image, uint32 lockFlags);

		void Clear();
		void Init(Image* image, uint32 lockFlags);

		uint8* GetData();
		const uint8* GetData() const;
		uint32 GetImageDataSize() const;
		uint32 GetLockFlags() const;
	private:
		friend class Image;
		Image* image_;
		uint32 lockFlags_;
	};


	class Image : public NonCopyable
	{
	public:
		enum ImageLockFlags
		{
			LockRead    = BIT(0),
			LockWrite   = BIT(1),
			LockRW      = LockRead | LockWrite
		};

		Image();
		Image(uint32 width, uint32 height, PixelFormat::Type format);
		Image(const void* data, uint32 length, PixelFormat::Type format);
		~Image();

		bool Valid() const { return data_ != nullptr; }
		uint32 GetWidth() const { return width_; }
		uint32 GetHeight() const { return height_; }
		PixelFormat::Type GetFormat() const { return format_; }

		uint32 GetBytesPerPixel() const { return bytesPerPixel_; }
		uint32 GetImageDataSize() const { return dataSize_; }

		// mutable image data, i.e., not loaded from an asset
		void Invalidate();
		void InvalidateRegion(const Recti& region);
		void Validate();
		const Recti& GetInvalidRegion() const { return invalidRegion_; }
		
		// lock image
		LockedImage* Lock(uint32 lockFlags);
		void Unlock(LockedImage* L);
		const uint8* GetConstData() const { return data_; }

		// image format must be PixelFormat::R8G8B8A8
		void SaveTGA(const string& path);
		// more format
		void SavePNG(const string& path, bool filpped = false);

		static uint32 GetBytesPerPixel(PixelFormat::Type format);
		static ImageRef FromFile(const string& path, PixelFormat::Type format = PixelFormat::R8G8B8A8);
		static ImageRef ToPixelFormat(const ImageRef& origin, PixelFormat::Type format);
	private:
		friend class LockedImage;
		uint8* GetData() { return data_; }
	private:
		uint32 width_;
		uint32 height_;
		PixelFormat::Type format_;
		uint32 bytesPerPixel_;
		uint32 dataSize_;
		uint8* data_;

		int32 lockedCount_;
		LockedImage lockedImage_;
		// mark for update later, e.g. for dynamic texture
		Recti invalidRegion_;
	};
}

#endif
