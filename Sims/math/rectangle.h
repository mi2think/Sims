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
		T left, top, right, bottom;

		Rectangle() : left(0), top(0), right(0), bottom(0) {}
		Rectangle(T l, T t, T r, T b) : left(l), top(t), right(r), bottom(b) {}
		Rectangle(const Rectangle<T>& rhs) { CopyRect(&rhs); }
		Rectangle(const Rectangle<T>* p) { CopyRect(p); }

		Rectangle& operator=(const Rectangle<T>& rhs)
		{
			CopyRect(&rhs);
			return *this;
		}
		Rectangle& operator&=(const Rectangle<T>& rhs) 
		{
			left = MAX(left, rhs.left);
			top = MAX(top, rhs.top);
			right = MIN(right, rhs.right);
			bottom = MIN(bottom, rhs.bottom);
			return *this;
		}
		Rectangle& operator|=(const Rectangle<T>& rhs)
		{
			left = MIN(left, rhs.left);
			top = MIN(top, rhs.top);
			right = MAX(right, rhs.right);
			bottom = MAX(bottom, rhs.bottom);
			return *this;
		}

		void CopyRect(const Rectangle<T>* src) { memcpy(this, src, sizeof(Rectangle<T>)); }
		void SetRect(T l, T t, T r, T b) { left = l; top = t; right = r; bottom = b; }

		T Width() const { return right - left; }
		T Height() const { return bottom - top; }
		void Width(T width) { right = left + width; }
		void Height(T height) { bottom = top + height; }
		Vector2f CenterPoint() const { return Vector2f((left + right) / 2.0f, (top + bottom) / 2.0f); }
		bool IsRectEmpty() const { return left >= right || top >= bottom; }
		
		template<class U> 
		bool PtInRect(const Vector2<U>& pt) const 
		{ 
			return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom; 
		}

		template<class U>
		void OffsetRect(U x, U y)
		{ 
			left += (T)x; right += (T)x; top += (T)y; bottom += (T)y;
		}

		template<class U>
		void OffsetRect(const Vector2<U>& pt)
		{ 
			OffsetRect(pt.x, pt.y); 
		}

		template<class U> 
		void InflateRect(U l, U t, U r, U b) 
		{ 
			left -= l; 
			right += r;
			top -= t;
			bottom += t;
		}
	};

	template<class T>
	bool operator==(const Rectangle<T>& lhs, const Rectangle<T>& rhs)
	{
		return equal_t(lhs.left, rhs.left) 
			&& equal_t(lhs.top, rhs.top)
			&& equal_t(lhs.right, rhs.right)
			&& equal_t(lhs.bottom, rhs.bottom);
	}

	template<class T>
	bool operator!=(const Rectangle<T>& lhs, const Rectangle<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template<class T>
	Rectangle<T> operator&(const Rectangle<T>& lhs, const Rectangle<T>& rhs)
	{
		Rectangle<T> rect = lhs;
		rect &= rhs;
		return rect;
	}
}

#endif
