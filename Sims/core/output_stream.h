/********************************************************************
	created:	2016/09/07
	created:	7:9:2016   22:28
	filename: 	D:\Code\Sims\Sims\output_stream.h
	file path:	D:\Code\Sims\Sims
	file base:	output_stream
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Output Stream
*********************************************************************/
#ifndef __OUTPUT_STREAM_H__
#define __OUTPUT_STREAM_H__

#include "sims.h"

namespace sims
{
	class IOutputStream
	{
	public:
		virtual ~IOutputStream() {}

		// write size bytes into buffer, return actually written bytes.
		virtual uint32 Write(const uint8* buffer, uint32 size) = 0;

		// flush stream
		virtual void Flush() = 0;

		// close stream
		virtual void Close() = 0;

		virtual bool IsClosed() const = 0;

		// seek to given position, relative to offset
		virtual void SetPosition(uint32 position, SeekType offset) = 0;

		// if has any IO errors
		virtual bool HasError() const = 0;
	};
}


#endif