/********************************************************************
	created:	2016/09/20
	created:	20:9:2016   10:55
	filename: 	D:\Code\Sims\Sims\math\rectangle.h
	file path:	D:\Code\Sims\Sims\math
	file base:	rectangle
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Rectangle
*********************************************************************/
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "math_fwd.h"
#include "vector2.h"

namespace sims
{
	template <class T> 
	class Rectangle
	{
	public:
		T x, y, w, h;

		Rectangle() : x(0), y(0), w(0), h(0) {}
		Rectangle(T _x, T _y, T _w, T _h) : x(_x), y(_y), w(_w), h(_h) {}
		Rectangle(const Rectangle<T>& rc) { CopyRect(&rc); }
		Rectangle(const Rectangle<T>* p) { CopyRect(p); }

		Rectangle& operator=(const Rectangle<T>& rc)
		{
			CopyRect(&rc);
			return *this;
		}
		Rectangle& operator&=(const Rectangle<T>& rc)
		{
			T r = MIN(x + w, rc.x + rc.w);
			T b = MIN(y + h, rc.y + rc.h);
			x = MAX(x, rc.x);
			y = MAX(y, rc.y);
			w = r - x;
			h = b - y;
			return *this;
		}
		Rectangle& operator|=(const Rectangle<T>& rc)
		{
			T r = MAX(x + w, rc.x + rc.w);
			T b = MAX(y + h, rc.y + rc.h);
			x = MIN(x, rc.x);
			y = MIN(y, rc.y);
			w = r - x;
			h = b - y;
			return *this;
		}

		void CopyRect(const Rectangle<T>* src) { memcpy(this, src, sizeof(Rectangle<T>)); }
		void SetRect(T _x, T _y, T _w, T _h) { x = _x; y = _y; w = _w; h = _h; }

		Vector2f CenterPoint() const { return Vector2f(x + w * 0.5f, y + h * 0.5f); }
		bool IsEmpty() const { return w <= 0 || h <= 0; }
		
		template<class U> 
		bool PtInRect(const Vector2<U>& pt) const 
		{ 
			return pt.x >= x && pt.x <= x + w && pt.y >= y && pt.y <= y + h;
		}

		template<class U>
		void OffsetRect(U _x, U _y)
		{
			x += _x;
			y += _y;
		}

		template<class U>
		void OffsetRect(const Vector2<U>& pt)
		{ 
			OffsetRect(pt.x, pt.y); 
		}
	};

	template<class T>
	bool operator==(const Rectangle<T>& rc1, const Rectangle<T>& rc2)
	{
		return equal_t(rc1.x, rc2.x)
			&& equal_t(rc1.y, rc2.y)
			&& equal_t(rc1.w, rc2.w)
			&& equal_t(rc1.h, rc2.h);
	}

	template<class T>
	bool operator!=(const Rectangle<T>& rc1, const Rectangle<T>& rc2)
	{
		return !(rc1 == rc2);
	}

	template<class T>
	Rectangle<T> operator&(const Rectangle<T>& rc1, const Rectangle<T>& rc2)
	{
		Rectangle<T> rc = rc1;
		rc &= rc2;
		return rc;
	}
}

#endif
