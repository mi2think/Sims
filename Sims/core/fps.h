/********************************************************************
	created:	2016/09/15
	created:	15:9:2016   15:30
	filename: 	D:\Code\Sims\Sims\core\fps.h
	file path:	D:\Code\Sims\Sims\core
	file base:	fps
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	FPS
*********************************************************************/
#pragma once

#include "sims.h"

namespace sims
{
	class Fps
	{
	public:
		Fps() : timeElapsed_(0.0), frameCount_(0), fps_(0.0f) {}

		void OnUpdate(const Timestep& timestep)
		{
			++frameCount_;
			timeElapsed_ += timestep.GetSeconds();
			if (timeElapsed_ >= 0.5f)
			{
				fps_ = frameCount_ / timeElapsed_;
				frameCount_ = 0;
				timeElapsed_ = 0.0f;
			}
		}
		float Count() const { return fps_; }
	private:
		float timeElapsed_;
		uint32 frameCount_;
		float fps_;
	};
}
