/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   21:18
	filename: 	D:\Code\Sims\Sims\core\file_input_stream.h
	file path:	D:\Code\Sims\Sims\core
	file base:	file_input_stream
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	File Input Stream
*********************************************************************/
#ifndef __FILE_INPUT_STREAM_H__
#define __FILE_INPUT_STREAM_H__

#include "input_stream.h"

namespace sims
{
	class FileInputStream : IInputStream
	{
	public:
		FileInputStream(FILE* fp, int size = -1);
		virtual ~FileInputStream();

		// get buffer size required to read the entire stream, return -1 if unknown
		virtual uint32 GetSize() const { return size_; }

		// read size bytes into buffer, return actually read size
		virtual uint32 Read(char* buffer, uint32 size);

		// get current read position in bytes from beginning of stream, return -1 if not supported
		virtual uint32 GetPosition() const;

		// seek to given position, relative to offset
		virtual void SetPosition(uint32 position, SeekType offset);
	private:
		FILE* fp_;
		uint32 size_;
	};
}


#endif