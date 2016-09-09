/********************************************************************
	created:	2016/09/09
	created:	9:9:2016   22:22
	filename: 	D:\Code\Sims\Sims\platform\platform.h
	file path:	D:\Code\Sims\Sims\platform
	file base:	platform
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Platform
*********************************************************************/
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "sims.h"

namespace sims
{
	class Platform
	{
	public:
		static IFileSystemRef GetFileSystem();
	};
}

#endif