/********************************************************************
	created:	2016/09/18
	created:	18:9:2016   16:16
	filename: 	D:\Code\Sims\Sims\core\file_system.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	file_system
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	File System
*********************************************************************/
#include "file_system.h"

namespace sims
{
	bool IFileSystem::Exists(const string& path)
	{
		FileInfo fi;
		return GetFileInfo(path, fi);
	}

	bool IFileSystem::FileExists(const string& path)
	{
		FileInfo fi;
		if (!GetFileInfo(path, fi))
		{
			return false;
		}
		return !fi.isDirectory;
	}

	bool IFileSystem::DirectoryExists(const string& path)
	{
		FileInfo fi;
		if (!GetFileInfo(path, fi))
		{
			return false;
		}
		return fi.isDirectory;
	}
}