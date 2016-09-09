/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   21:49
	filename: 	D:\Code\Sims\Sims\platform\windows\windows_file_system.h
	file path:	D:\Code\Sims\Sims\platform\windows
	file base:	windows_file_system
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Windows File System
*********************************************************************/
#ifndef __WINDOWS_FILE_SYSTEM_H__

#include "sims.h"
#include "core/file_system.h"

namespace sims
{
	class WindowsFileSystem : public IFileSystem
	{
	public:
		// open a file for reading, return nullptr if can not open
		virtual IInputStreamRef OpenInputStream(const string& path);

		// open a file for writing, return nullptr if can not open
		virtual IOutputStreamRef OpenOutputStream(const string& path);

		// get file info, return false if not exists
		virtual bool GetFileInfo(const string& path, FileInfo& fi) const;
	};
}

#endif
