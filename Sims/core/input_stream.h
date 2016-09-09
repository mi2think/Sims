/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   20:43
	filename: 	D:\Code\Sims\Sims\core\input_stream.h
	file path:	D:\Code\Sims\Sims\core
	file base:	input_stream
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Input Stream
*********************************************************************/
#ifndef __INPUT_STREAM_H__
#define __INPUT_STREAM_H__

#include "sims.h"

namespace sims
{
	class IInputStream
	{
	public:
		virtual ~IInputStream() {}

		// get buffer size required to read the entire stream, return -1 if unknown
		virtual uint32 GetSize() const = 0;

		// read size bytes into buffer, return actually read size
		virtual uint32 Read(uint8* buffer, uint32 size) = 0;

		// read the entire stream
		virtual vector<uint8> Read();
		virtual void Read(vector<uint8>& buffer);

		// get current read position in bytes from beginning of stream, return -1 if not supported
		virtual uint32 GetPosition() const = 0;

		// seek to given position, relative to offset
		virtual void SetPosition(uint32 position, SeekType offset) = 0;
	};
}

#endif