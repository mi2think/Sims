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
#include "windows_platform.h"
#include "windows_file_system.h"
#include "core/path.h"

#include <windows.h>

namespace sims
{
	void CheckWinError(const char* file, int line)
	{
		DWORD error = GetLastError();
		if (error == 0)
			return;

		char buf[128] = { 0 };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, error,
			LANG_SYSTEM_DEFAULT, buf, sizeof(buf), nullptr);
		const char* str = str_format("%s(%d):\n%s\n", file, line, buf);
		MessageBox(nullptr, str, "error", MB_OK);
	}

	static WindowsFileSystem s_fileSystem;

	IFileSystemRef Platform::GetFileSystem()
	{
		IFileSystemRef fs(&s_fileSystem);
		fs.IncRef();
		return fs;
	}

	string Platform::GetEnv(const string& name)
	{
		const int32 bufferSize = 512;
		vector<char> buffer;
		buffer.resize(bufferSize);

		string retVal;
		DWORD val = GetEnvironmentVariable(name.c_str(), &buffer[0], bufferSize);
		if (val == 0)
		{
			CHECK_WIN_ERROR();
			return retVal;
		}
		else if (bufferSize < val)
		{
			buffer.resize(val);
			val = GetEnvironmentVariable(name.c_str(), &buffer[0], val);
			if (!val)
			{
				CHECK_WIN_ERROR();
				return retVal;
			}
		}
		retVal.assign(&buffer[0], &buffer[0] + val);
		return retVal;
	}

	bool Platform::SetEnv(const string& name, const string& val)
	{
		if (!SetEnvironmentVariable(name.c_str(), val.c_str()))
		{
			CHECK_WIN_ERROR();
			return false;
		}
		return true;
	}

	string& Platform::GetBaseDir()
	{
		static string baseDir(512, '\0');

		if (baseDir[0])
			return baseDir;
		else
		{
			HMODULE currModule = nullptr;
			if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&GetBaseDir, &currModule))
			{
				LOG_ERROR("Platform GetBaseDir failed! Error Code: %d", GetLastError());
			}
			else
			{
				GetModuleFileName(currModule, &baseDir[0], baseDir.size());
				baseDir = Path::Normalize(baseDir);
				baseDir = Path::Parent(baseDir);
			}
		}

		return baseDir;
	}

	void Platform::LowLevelOutputDebugString(const char* s)
	{
		::OutputDebugString(s);
	}
}
