/********************************************************************
	created:	2016/09/25
	created:	25:9:2016   15:15
	filename: 	D:\Code\Sims\Sims\math\rng.h
	file path:	D:\Code\Sims\Sims\math
	file base:	rng
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Random Number Generator
*********************************************************************/
#ifndef __RNG_H__
#define __RNG_H__

#include "sims.h"
#include <random>

namespace sims
{
	class RNG
	{
	public:
		RNG(uint32 seed);

		void Seed(uint32 seed);

		int32  RandomInt32() const;
		uint32 RandomUInt32() const;
		float  RandomFloat() const;

		int32  RandomInt32(int32 imin, int32 imax) const;
		uint32 RandomUInt32(uint32 umin, uint32 umax) const;
		float  RandomFloat(float fmin, float fmax) const;

		void   RandomInt32(int32* buffer, int32 bufferSize, int32 imin, int32 imax) const;
		void   RandomUInt32(uint32* buffer, uint32 bufferSize, uint32 umin, uint32 umax) const;
		void   RandomFloat(float* buffer, uint32 bufferSize, float fmin, float fmax) const;
	private:
		mutable std::default_random_engine engine_;
	};
}


#endif