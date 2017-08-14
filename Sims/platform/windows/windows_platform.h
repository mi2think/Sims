/********************************************************************
	created:	2017/08/14
	created:	14:8:2017   15:44
	filename: 	D:\Code\Sims\Sims\platform\windows\windows_platform.h
	file path:	D:\Code\Sims\Sims\platform\windows
	file base:	windows_platform
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Windows Platform
*********************************************************************/
#pragma once

#include "core/platform.h"

namespace sims
{
	void CheckWinError(const char* file, int line);
	#define CHECK_WIN_ERROR()  CheckWinError(__FILE__, __LINE__)
}
