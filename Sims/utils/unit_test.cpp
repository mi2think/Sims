/********************************************************************
	created:	2016/09/05
	created:	5:9:2016   11:43
	filename: 	D:\Code\Sims\Sims\utils\unit_test.cpp
	file path:	D:\Code\Sims\Sims\utils
	file base:	unit_test
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Unit Test
*********************************************************************/
#include "unit_test.h"

namespace sims
{
	TestCase::TestCase(const char* testCaseName)
		: testCaseName_(testCaseName)
		, nPassed_(0)
		, nFailed_(0)
	{
	}

	TestCase::~TestCase()
	{
	}

	UnitTest::UnitTest()
		: nPassed_(0)
		, nFailed_(0)
		, currTestCase_(nullptr)
	{
	}

	UnitTest::~UnitTest()
	{
		for (auto e : testCases_)
		{
			delete e;
		}
	}

	TestCase* UnitTest::RegisterTestCase(TestCase* testCase)
	{
		testCases_.push_back(testCase);
		return testCase;
	}

	void UnitTest::Run()
	{
		for (auto e : testCases_)
		{
			currTestCase_ = e;
			e->Run();

			nPassed_ += e->GetPassed();
			nFailed_ += e->GetFailed();

			LOG_INFO(">>>>>>>> %s", currTestCase_->GetTestCaseName());
			LOG_INFO("*************************************");
		}

		LOG_INFO("Run Tests:%d", nPassed_ + nFailed_);
		LOG_INFO("Passed:%d", nPassed_);

		if (nFailed_ > 0)
			LOG_ERROR("Failed:%d", nFailed_);
		else
			LOG_INFO("Failed:%d", nFailed_);
	}

	void UnitTest::MarkCurrentTestCase(bool passed)
	{
		TestCase* currTestCase = UnitTest::GetInstance().GetCurrentTestCase();
		if (passed)
			++currTestCase->GetPassed();
		else
			++currTestCase->GetFailed();
	}
}