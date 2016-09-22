/********************************************************************
	created:	2016/09/07
	created:	7:9:2016   20:02
	filename: 	D:\Code\Sims\Sims\color.h
	file path:	D:\Code\Sims\Sims
	file base:	color
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Color
*********************************************************************/
#ifndef __COLOR_H__
#define __COLOR_H__

#include "sims.h"

namespace sims
{
	class Color
	{
	public:
		union
		{
			struct 
			{
				uint8 b;
				uint8 g;
				uint8 r;
				uint8 a;
			};
			uint32 value;
		};
		Color() {}
		Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 255)
			: r(_r)
			, g(_g)
			, b(_b)
			, a(_a)
		{}
		Color(uint32 val) : value(val) {}

		Color& operator*=(float k)
		{
			r = (uint8)clamp_t(r * k, 0.0f, 255.0f);
			g = (uint8)clamp_t(g * k, 0.0f, 255.0f);
			b = (uint8)clamp_t(b * k, 0.0f, 255.0f);
			a = (uint8)clamp_t(a * k, 0.0f, 255.0f);
			return *this;
		}

		Color& operator+=(const Color& c)
		{
			r = MIN(r + c.r, 255);
			g = MIN(g + c.g, 255);
			b = MIN(b + c.b, 255);
			a = MIN(a + c.a, 255);
			return *this;
		}

		Color ToPackedARGB() const
		{
			return Color((a << 24) | (r << 16) | (g << 8) | b);
		}

		Color ToPackedRGBA() const
		{
			return Color((r << 24) | (g << 16) | (b << 8) | a);
		}

		static const Color c_white;
		static const Color c_gray;
		static const Color c_black;
		static const Color c_red;
		static const Color c_green;
		static const Color c_blue;
	};

	inline Color operator*(const Color& c, float k)
	{
		Color color = c;
		return color *= k;
	}

	inline Color operator*(float k, const Color& c)
	{
		Color color = c;
		return color *= k;
	}
}

#endif
