/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   22:44
	filename: 	D:\Code\Sims\Sims\sims.h
	file path:	D:\Code\Sims\Sims
	file base:	sims
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	
*********************************************************************/
#ifndef __SIMS_H__
#define __SIMS_H__

#include "math/math_fwd.h"
#include "core/log.h"
#include "core/ref.h"
#include "utils/string_utils.h"

#include <sstream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cassert>

namespace sims
{
	// macros
#define SAFE_DELETE(p)		do { if (p != nullptr) { delete p; p = nullptr; } } while(0)
#define SAFE_DELETEARRAY(p) do { if (p != nullptr) { delete[] p; p = nullptr; } } while(0)
#define SAFE_RELEASE(p)		do { if (p != nullptr) { p->Release(); p = nullptr; } } while(0)

#define ASSERT	assert
#define BIT(x)	(1<<(x))

#define DECLARE_ENUM(name) \
		namespace name { \
			enum Type {

#define END_DECLARE_ENUM() \
				,Max \
			}; \
		}

	using std::vector;
	using std::map;
	using std::pair;
	using std::ostringstream;

	class Event;
	typedef Ref<Event> EventRef;

	class NonCopyable
	{
	public:
		NonCopyable() {}
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};

	class ScopeGuard : public NonCopyable
	{
	public:
		ScopeGuard(std::function<void()> func) : func_(func) {}
		~ScopeGuard() { func_(); }
	private:
		std::function<void()> func_;
	};
#define SCOPEGUARD_NAMELINE_CAT(name, line) name##line
#define SCOPEGUARD_NAMELINE(name, line) SCOPEGUARD_NAMELINE_CAT(name, line)
#define ON_SCOPE_EXIT(func) ScopeGuard SCOPEGUARD_NAMELINE(EXIT, __LINE__)(func)

	// time step
	class Timestep
	{
	public:
		explicit Timestep(int milliseconds) : milliseconds_(milliseconds) {}

		int GetMilliseconds() const { return milliseconds_; }
		float GetSeconds() const { return milliseconds_ * 0.001f; }
	private:
		int milliseconds_;
	};
}

#endif