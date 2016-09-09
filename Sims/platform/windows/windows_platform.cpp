/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   22:32
	filename: 	D:\Code\Sims\Sims\platform\windows\windows_platform.cpp
	file path:	D:\Code\Sims\Sims\platform\windows
	file base:	windows_platform
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Windows Platform
*********************************************************************/
#include "platform/platform.h"
#include "windows_file_system.h"

namespace sims
{
	static WindowsFileSystem s_fileSystem;

	IFileSystemRef Platform::GetFileSystem()
	{
		IFileSystemRef fs(&s_fileSystem);
		fs.IncRef();
		return fs;
	}
}