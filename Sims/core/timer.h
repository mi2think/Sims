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
		float GetTimestep() const { return timestep_; }
		float Tick()
		{
			currTime_ = std::chrono::high_resolution_clock::now();
			timestep_ = std::chrono::duration<float>(lastTime_ - currTime_).count();
			return timestep_;
		}
		void Reset() { lastTime_ = currTime_; }
	private:
		std::chrono::high_resolution_clock::time_point currTime_;
		std::chrono::high_resolution_clock::time_point lastTime_;
		float timestep_; // s
	};
}

#endif
