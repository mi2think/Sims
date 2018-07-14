/********************************************************************
	created:	2017/08/14
	created:	14:8:2017   11:24
	filename: 	D:\Code\Sims\Sims\core\platform.h
	file path:	D:\Code\Sims\Sims\core
	file base:	platform
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Platform
*********************************************************************/
#pragma once

#include "sims.h"

namespace sims
{
	class Platform
	{
	public:
		static IFileSystemRef GetFileSystem();

		// environment
		static string GetEnv(const string& name);
		static bool SetEnv(const string& name, const string& val);

		static string& GetBaseDir();

		static void LowLevelOutputDebugString(const char* s);
	};
}
