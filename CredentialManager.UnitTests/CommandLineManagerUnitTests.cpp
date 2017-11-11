#include "includes.h"

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CredentialManagerUnitTests
{
    TEST_CLASS(CommandLineManagerUnitTests)
    {
    public:
        wchar_t* FirstArg() { return L""; }

        TEST_METHOD(NoArgs_Parsed_ShowHelp)
        {
            auto ctx = CallParse({ FirstArg() });

            Assert::IsTrue(ctx.m_showHelp);
        }

        TEST_METHOD(HelpArg_Parsed_ShowHelp)
        {
            auto ctx = CallParse({ FirstArg(), L"-?" });

            Assert::IsTrue(ctx.m_showHelp);
        }

        TEST_METHOD(HelpArgAndAnythingElse_Parsed_ShowHelp)
        {
            auto ctx = CallParse({ FirstArg(), L"-?", L"abcdef" });

            Assert::IsTrue(ctx.m_showHelp);
        }

        TEST_METHOD(CArg_Parsed_ShowConfiguration)
        {
            auto ctx = CallParse({ FirstArg(), L"-c" });

            Assert::IsTrue(ctx.m_showConfiguration);
        }

        TEST_METHOD(CArgAndAnythingElse_Parsed_ShowConfiguration)
        {
            auto ctx = CallParse({ FirstArg(), L"-c", L"abcdef" });

            Assert::IsTrue(ctx.m_showConfiguration);
        }


        TEST_METHOD(GArg_Parsed_GetPasswordCommandCreated)
        {
            auto ctx = CallParse({ FirstArg(), L"-g", L"Name" });

            Assert::AreEqual<size_t>(1, ctx.m_commands.size());
            Assert::AreEqual<int>((int)AppCommandType::GetPassword, (int)ctx.m_commands[0].m_commandType);
            Assert::AreEqual(std::wstring(L"Name"), ctx.m_commands[0].m_name);
            Assert::AreEqual(std::wstring(L""), ctx.m_commands[0].m_password);
        }


    private:
        AppContext CallParse(std::initializer_list<wchar_t*> args)
        {
            CommandLineManager clm{};
            return clm.ParseCommandLine(args.size(), args.begin());
        }
    };
}