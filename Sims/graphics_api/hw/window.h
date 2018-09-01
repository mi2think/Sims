/********************************************************************
	created:	2017/04/12
	created:	12:4:2017   2:49
	filename: 	D:\Code\Sims\Sims\graphics_api\hw\window.h
	file path:	D:\Code\Sims\Sims\graphics_api\hw
	file base:	window
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Hardware Window
*********************************************************************/
#pragma once

namespace sims
{
	namespace hw
	{
		template<typename ActualWindow>
		class Window : public ActualWindow
		{
		};
	}
}

