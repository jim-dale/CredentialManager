#pragma once

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CredentialManagerUnitTests
{
    TEST_CLASS(AppMetadataUnitTests)
    {
    public:
        TEST_METHOD(CheckSourceVersion_Equals_dashNOTSETdash)
        {
            std::wstring expected(L"-NOTSET-");
            std::wstring actual(Check_SourceVersion);

            Assert::AreEqual(expected, actual, L"Check_SourceVersion must be set to '-NOTSET-'");
        }
    };
}
