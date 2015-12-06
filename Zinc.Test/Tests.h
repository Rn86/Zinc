#pragma once

#define ZN_TEST_CLASS TEST_CLASS
#define ZN_TEST_METHOD TEST_METHOD

namespace Zinc { namespace Tests {

	class Assert
	{
	public:
		static void IsTrue(bool && condition)
		{
			Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(condition);
		}
	};

}}
