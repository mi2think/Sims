/********************************************************************
	created:	2017/12/18
	created:	18:12:2017   15:27
	filename: 	D:\Code\Sims\Sims\core\app.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	app
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	App
*********************************************************************/
#include "app.h"
#include "vfs.h"
#include "platform.h"

namespace sims
{
	void InitApp()
	{
		VFS::Init();

		LOG_INFO("Working DIR: %s", Platform::GetBaseDir().c_str());
	}

	void ExitApp()
	{
		VFS::Shutdown();
	}
}