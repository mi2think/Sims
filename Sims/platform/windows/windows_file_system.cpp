/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   21:54
	filename: 	D:\Code\Sims\Sims\platform\windows\windows_file_system.cpp
	file path:	D:\Code\Sims\Sims\platform\windows
	file base:	windows_file_system
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Windows File System
*********************************************************************/
#include "windows_file_system.h"
#include "core/file_input_stream.h"
#include "core/file_output_stream.h"

#include <windows.h>

namespace sims
{
	IInputStreamRef WindowsFileSystem::OpenInputStream(const string& path)
	{
		FileInfo fi;
		if (!GetFileInfo(path, fi) || fi.isDirectory)
		{
			return IInputStreamRef();
		}

		FILE* fp = nullptr;
		fopen_s(&fp, path.c_str(), "rb");
		if (!fp)
		{
			return IInputStreamRef();
		}

		return IInputStreamRef(new FileInputStream(fp, fi.size));
	}

	IOutputStreamRef WindowsFileSystem::OpenOutputStream(const string& path)
	{
		// TODO:maybe need create parent directory

		FILE* fp = nullptr;
		fopen_s(&fp, path.c_str(), "wb");
		if (!fp)
		{
			return IOutputStreamRef();
		}
		return IOutputStreamRef(new FileOutputStream(fp));
	}

	bool WindowsFileSystem::GetFileInfo(const string& path, FileInfo& fi) const
	{
		WIN32_FILE_ATTRIBUTE_DATA fileInfo;	
		if (GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fileInfo))
		{
			fi.isDirectory = ((fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
			fi.size = fileInfo.nFileSizeLow;
			ASSERT(fileInfo.nFileSizeHigh == 0); // <=4G support temporary
			return true;
		}
		return false;
	}
}