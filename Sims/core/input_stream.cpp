/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   20:57
	filename: 	D:\Code\Sims\Sims\core\input_stream.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	input_stream
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Input Stream
*********************************************************************/
#include "input_stream.h"

namespace sims
{
	Buffer IInputStream::Read()
	{
		Buffer buffer;
		Read(buffer);
		return buffer;
	}

	void IInputStream::Read(Buffer& buffer)
	{
		uint32 size = GetSize();
		uint32 position = GetPosition();
		if (size != -1 && position != -1)
		{
			// read it once
			buffer.Resize(size - position);
			Read(buffer.GetData(), size - position);
		}
		else
		{
			// read it incrementally
			uint32 blockSize = 4096;
			uint32 readSize = 0;
			do 
			{
				blockSize += blockSize;
				buffer.Resize(buffer.GetSize() + blockSize);
				readSize = Read(&buffer[buffer.GetSize() - blockSize], blockSize);
			} while (readSize == blockSize);

			buffer.Resize(buffer.GetSize() - (blockSize - readSize));
		}
	}
}