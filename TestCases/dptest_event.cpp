#include "core/event.h"
#include "utils/unit_test.h"
using namespace sims;

extern const char s1[] = "UserEvent";
extern const char s2[] = "MyEvent2";

UNIT_TEST(Event)
{
	enum
	{
		USER_EVENT1 = Event::USER + 1,
		USER_EVENT2,
		USER_EVENT3,
		USER_EVENT4,
		USER_EVENT5,
		USER_EVENT6
	};

	// const EventType* event_cast(const Event& event)
	{
		class MyEvent1 : public BaseEvent<USER_EVENT1, s1>
		{
		public:
			MyEvent1(int x, int y)
				: x_(x)
				, y_(y)
			{}

			int GetX() const { return x_; }
			int GetY() const { return y_; }
		private:
			int x_;
			int y_;
		};

		auto OnMyEvent1 = [](const Event& event)
		{
			const MyEvent1* myEvent = event_cast<MyEvent1>(event);
			EXPECT_TRUE(myEvent != nullptr);
			EXPECT_EQ(myEvent->GetX(), 1);
			EXPECT_EQ(myEvent->GetY(), 2);
			EXPECT_EQ(myEvent->ToString(), s1);
		};
		MyEvent1 myEvent1(1, 2);
		OnMyEvent1(myEvent1);
	}

	// Ref<EventType> event_cast(const EventRef& event)
	{
		class MyEvent2 : public BaseEvent<USER_EVENT2, s2>
		{
		public:
			MyEvent2(const char* id) : id_(id) {}

			const char* GetID() const { return id_; }
		private:
			const char* id_;
		};

		typedef Ref<MyEvent2> MyEvent2Ref;

		auto OnMyEvent2 = [](const EventRef& event)
		{
			const MyEvent2Ref myEvent2 = event.As<MyEvent2>();
			EXPECT_TRUE(strcmp("Hello world", myEvent2->GetID()) == 0);
		};
		EventRef myEvent2(new MyEvent2("Hello world"));
		OnMyEvent2(myEvent2);
	}

	// EventDispatch
	{
		class MyEvent3 : public BaseEvent<USER_EVENT3>
		{
		public:
			MyEvent3() {}
		};

		class MyEvent4 : public BaseEvent<USER_EVENT4>
		{
		public:
			MyEvent4() {}
		};

		auto OnMyEvent3 = [](const MyEvent3& event) -> bool
		{
			return true;
		};

		auto OnMyEvent4 = [](const MyEvent4& event) -> bool
		{
			return true;
		};

		MyEvent3 myEvent3;
		MyEvent4 myEvent4;
		EventDispatch dispatch(myEvent3);
		dispatch.Dispatch((bool (*)(const MyEvent4&))OnMyEvent4);
		EXPECT_TRUE(dispatch.GetResult() == false);
		dispatch.Dispatch((bool(*)(const MyEvent3&))OnMyEvent3);
		EXPECT_TRUE(dispatch.GetResult() == true);
	}

	// IEventListener
	{
		class MyEvent5 : public BaseEvent<USER_EVENT5>
		{
		public:
			MyEvent5(int a) : a_(a) {}

			int GetParam() const { return a_; }
		private:
			int a_;
		};

		class UserHandle : public IEventListener
		{
		public:
			UserHandle(int a) : a_(a) {}

			bool OnEvent(const Event& event)
			{
				EventDispatch dispatch(event);
				dispatch.Dispatch(this, &UserHandle::OnEvent5);
				return dispatch.GetResult();
			}

			bool OnEvent5(const MyEvent5& event)
			{
				a_ += event.GetParam();
				return true;
			}

			int GetParam() const { return a_; }
		private:
			int a_;
		};

		auto OnEvent = [](IEventListener* listener, const Event& event)
		{
			if (listener)
				listener->OnEvent(event);
		};

		MyEvent5 myEvent5(1);
		UserHandle userHandle(2);
		OnEvent(&userHandle, myEvent5);
		EXPECT_TRUE(userHandle.GetParam() == 3);
	}

	// Event broadcaster
	{
		class MyEvent6 : public BaseEvent<USER_EVENT6>
		{
		public:
			MyEvent6(int a) : a_(a) {}
			
			int GetParam() const { return a_; }
		private:
			int a_;
		};

		class UserHandle1 : public IEventListener
		{
		public:
			UserHandle1(int a) : a_(a) {}

			bool OnEvent(const Event& event)
			{
				EventDispatch dispatch(event);
				dispatch.Dispatch(this, &UserHandle1::OnEvent6);
				return dispatch.GetResult();
			}

			bool OnEvent6(const MyEvent6& event)
			{
				a_ += event.GetParam();
				return true;
			}

			int GetParam() const { return a_; }
		private:
			int a_;
		};

		class UserHandle2 : public IEventListener
		{
		public:
			UserHandle2(int a) : a_(a) {}

			bool OnEvent(const Event& event)
			{
				EventDispatch dispatch(event);
				dispatch.Dispatch(this, &UserHandle2::OnEvent6);
				return dispatch.GetResult();
			}

			bool OnEvent6(const MyEvent6& event)
			{
				a_ -= event.GetParam();
				return true;
			}

			int GetParam() const { return a_; }
		private:
			int a_;
		};

		class UserHandle3 : public IEventListener, public EventBroadcaster
		{
		public:
			UserHandle3(int a) : a_(a) {}

			bool OnEvent(const Event& event)
			{
				EventDispatch dispatch(event);
				dispatch.Dispatch(this, &UserHandle3::OnEvent6);
				return dispatch.GetResult();
			}

			bool OnEvent6(const MyEvent6& event)
			{
				a_ *= event.GetParam();
				return true;
			}

			int GetParam() const { return a_; }
		private:
			int a_;
		};

		UserHandle1 handle1(1);
		UserHandle2 handle2(2);
		UserHandle3 handle3(3);

		handle3.RegisterEventListener(&handle1);
		handle3.RegisterEventListener(&handle2);
		handle3.RegisterEventListener(&handle3);
		handle3.PostEvent(MyEvent6(6));
		EXPECT_TRUE(handle1.GetParam() == 7);
		EXPECT_TRUE(handle2.GetParam() == -4);
		EXPECT_TRUE(handle3.GetParam() == 18);
	}
}
