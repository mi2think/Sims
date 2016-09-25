/********************************************************************
	created:	2016/09/25
	created:	25:9:2016   15:21
	filename: 	D:\Code\Sims\Sims\math\rng.cpp
	file path:	D:\Code\Sims\Sims\math
	file base:	rng
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Random Number Generator
*********************************************************************/
#include "rng.h"

namespace sims
{
	RNG::RNG(uint32 seed)
		: engine_(seed)
	{
	}

	void RNG::Seed(uint32 seed)
	{
		engine_.seed(seed);
	}

	int32 RNG::RandomInt32() const
	{
		std::uniform_int_distribution<int32> u;
		return u(engine_);
	}

	int32 RNG::RandomInt32(int32 imin, int32 imax) const
	{
		std::uniform_int_distribution<int32> u(imin, imax);
		return u(engine_);
	}

	void RNG::RandomInt32(int32* buffer, int32 bufferSize, int32 imin, int32 imax) const
	{
		std::uniform_int_distribution<int32> u(imin, imax);
		for (int32 i = 0; i < bufferSize; ++i)
		{
			*(buffer + i) = u(engine_);
		}
	}

	uint32 RNG::RandomUInt32() const
	{
		std::uniform_int_distribution<uint32> u;
		return u(engine_);
	}

	uint32 RNG::RandomUInt32(uint32 umin, uint32 umax) const
	{
		std::uniform_int_distribution<uint32> u(umin, umax);
		return u(engine_);
	}

	void RNG::RandomUInt32(uint32* buffer, uint32 bufferSize, uint32 umin, uint32 umax) const
	{
		std::uniform_int_distribution<uint32> u(umin, umax);
		for (uint32 i = 0; i < bufferSize; ++i)
		{
			*(buffer + i) = u(engine_);
		}
	}

	float RNG::RandomFloat() const
	{
		std::uniform_real_distribution<float> u;
		return u(engine_);
	}

	float RNG::RandomFloat(float fmin, float fmax) const
	{
		std::uniform_real_distribution<float> u(fmin, fmax);
		return u(engine_);
	}

	void RNG::RandomFloat(float* buffer, uint32 bufferSize, float fmin, float fmax) const
	{
		std::uniform_real_distribution<float> u(fmin, fmax);
		for (uint32 i = 0; i < bufferSize; ++i)
		{
			*(buffer + i) = u(engine_);
		}
	}
}