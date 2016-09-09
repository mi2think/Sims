/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   21:23
	filename: 	D:\Code\Sims\Sims\core\file_input_stream.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	file_input_stream
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	File Input Stream
*********************************************************************/
#include "file_input_stream.h"

namespace sims
{
	FileInputStream::FileInputStream(FILE* fp, int size /* = -1 */)
		: fp_(fp)
		, size_(size)
	{}

	FileInputStream::~FileInputStream()
	{
		fclose(fp_);
	}

	uint32 FileInputStream::Read(uint8* buffer, uint32 size)
	{
		return fread(buffer, 1, size, fp_);
	}

	uint32 FileInputStream::GetPosition() const
	{
		return ftell(fp_);
	}

	void FileInputStream::SetPosition(uint32 position, SeekType offset)
	{
		fseek(fp_, position, offset);
	}
}