/********************************************************************
	created:	2016/09/07
	created:	7:9:2016   23:04
	filename: 	D:\Code\Sims\Sims\core\file_output_stream.h
	file path:	D:\Code\Sims\Sims\core
	file base:	file_output_stream
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	File Output Stream
*********************************************************************/
#ifndef __FILE_OUTPUT_STREAM_H__
#define __FILE_OUTPUT_STREAM_H__

#include "sims.h"
#include "output_stream.h"

namespace sims
{
	class FileOutputStream : public IOutputStream
	{
	public:
		FileOutputStream(FILE* fp);
		virtual ~FileOutputStream();

		// write size bytes into buffer, return actually written bytes.
		virtual uint32 Write(const char* buffer, uint32 size);

		// flush stream
		virtual void Flush();

		// close stream
		virtual void Close();

		virtual bool IsClosed() const { return !fp_; }

		// return position relative begin of stream, return -1 if failed
		virtual int32 GetPosition() const;

		// seek to given position, relative to offset
		virtual void SetPosition(uint32 position, SeekType offset);

		// if has any IO errors
		virtual bool HasError() const { return hasError_; }
	private:
		FILE* fp_;
		mutable bool hasError_;
	};
}

#endif
