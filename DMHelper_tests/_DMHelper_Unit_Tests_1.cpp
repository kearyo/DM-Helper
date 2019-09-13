#include "stdafx.h"
#include "CppUnitTest.h"
#include "resource.h"
#include "DMUnitTestDialog.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DMHelper_tests_1
{		
	TEST_CLASS(DMHelper_Unit_Tests)
	{
	public:

		TEST_METHOD(DXSoundTest)
		{
			SoundClass m_SoundClass;

			bool bSuccess = m_SoundClass.Initialize(NULL);

			Assert::IsFalse(bSuccess);

			HWND hHwnd = GetDesktopWindow();

			bSuccess = m_SoundClass.Initialize(hHwnd);

			Assert::IsTrue(bSuccess);

			bSuccess = m_SoundClass.PlayDXSoundFromFile("C:\\BAD_PATH.WAV");

			Assert::IsFalse(bSuccess);

			bSuccess = m_SoundClass.PlayDXSoundFromFile("D:\\source\\DM Helper\\Release\\Data\\sounds\\magic_weapon_hits\\magic-strike-fast.wav");

			Assert::IsTrue(bSuccess);

			m_SoundClass.Shutdown();

			Assert::AreEqual(m_SoundClass.GetBufferedSoundCount(), 0);
		}

	};

	
}