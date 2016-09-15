/********************************************************************
	created:	2016/09/05
	created:	5:9:2016   0:29
	filename: 	D:\Code\Sims\Sims\core\timer.h
	file path:	D:\Code\Sims\Sims\core
	file base:	timer
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Timer
*********************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>

namespace sims
{
	class Timer
	{
	public:
		uint32 GetTimestep() const { return timestep_; }
		uint32 Tick()
		{
			currTime_ = std::chrono::high_resolution_clock::now();
			timestep_ = (uint32)std::chrono::duration_cast<std::chrono::milliseconds>(currTime_ - lastTime_).count();
			return timestep_;
		}
		void Reset() { lastTime_ = currTime_; }
	private:
		std::chrono::high_resolution_clock::time_point currTime_;
		std::chrono::high_resolution_clock::time_point lastTime_;
		uint32 timestep_; // ms
	};
}

#endif
