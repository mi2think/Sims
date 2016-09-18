/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   21:41
	filename: 	D:\Code\Sims\Sims\core\file_system.h
	file path:	D:\Code\Sims\Sims\core
	file base:	file_system
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	File System
*********************************************************************/
#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include "sims.h"

namespace sims
{
	class IFileSystem
	{
	public:
		struct FileInfo 
		{
			uint32 size;
			bool isDirectory;
		};

		virtual ~IFileSystem() {}

		// open a file for reading, return nullptr if can not open
		virtual IInputStreamRef OpenInputStream(const string& path) = 0;

		// open a file for writing, return nullptr if can not open
		virtual IOutputStreamRef OpenOutputStream(const string& path) = 0;

		// get file info, return false if not exists
		virtual bool GetFileInfo(const string& path, FileInfo& fi) const = 0;

		// whether a file or directory exists
		virtual bool Exists(const string& path);

		// whether a file exists
		virtual bool FileExists(const string& path);

		// whether a Directory exists
		virtual bool DirectoryExists(const string& path);

		// delete file at given path
		virtual bool DeleteFile(const string& path) = 0;
	};
}

#endif