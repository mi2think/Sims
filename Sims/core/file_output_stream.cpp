/********************************************************************
	created:	2016/09/07
	created:	7:9:2016   23:13
	filename: 	D:\Code\Sims\Sims\core\file_output_stream.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	file_output_stream
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	File Output Stream
*********************************************************************/
#include "file_output_stream.h"

namespace sims
{
	FileOutputStream::FileOutputStream(FILE* fp)
		: fp_(fp)
		, hasError_(false)
	{}

	FileOutputStream::~FileOutputStream()
	{
		if (!IsClosed())
			Close();
	}

	uint32 FileOutputStream::Write(const uint8* buffer, uint32 size)
	{
		uint32 result = fwrite(buffer, 1, size, fp_);
		if (result != size)
			hasError_ = true;
		return result;
	}

	void FileOutputStream::Flush()
	{
		int result = fflush(fp_);
		if (result == EOF)
		{
			hasError_ = true;
			LOG_ERROR("error in FileOutputStream::Flush!");
		}
	}

	void FileOutputStream::Close()
	{
		if (IsClosed())
			return;
		int result = fclose(fp_);
		if (result == EOF)
		{
			hasError_ = true;
			LOG_ERROR("error in FileOutputStream::Close!");
		}
		fp_ = nullptr;
	}

	int32 FileOutputStream::GetPosition() const
	{
		int32 result = ftell(fp_);
		if (result == -1L)
		{
			hasError_ = true;
			LOG_ERROR("error in FileOutputStream::GetPosition!");
		}
		return result;
	}

	void FileOutputStream::SetPosition(uint32 position, SeekType offset)
	{
		int32 result = fseek(fp_, position, offset);
		if (result != 0)
		{
			hasError_ = true;
			LOG_ERROR("error in FileOutputStream::SetPosition!");
		}
	}
}