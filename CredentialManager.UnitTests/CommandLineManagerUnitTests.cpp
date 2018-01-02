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
        }

        TEST_METHOD(LArgAndNothingElse_Parsed_ShowAll)
        {
            auto ctx = CallParse({ FirstArg(), L"-l" });

            Assert::AreEqual<size_t>(1, ctx.m_commands.size());
            Assert::AreEqual<int>((int)AppCommandType::ListEntries, (int)ctx.m_commands[0].m_commandType);
            Assert::AreEqual(Constants::DefaultFilter, ctx.m_commands[0].m_name);
        }

        TEST_METHOD(LArgAndAnythingElse_Parsed_ShowFiltered)
        {
            auto ctx = CallParse({ FirstArg(), L"-l", L"filter*" });

            Assert::AreEqual<size_t>(1, ctx.m_commands.size());
            Assert::AreEqual<int>((int)AppCommandType::ListEntries, (int)ctx.m_commands[0].m_commandType);
            Assert::AreEqual(std::wstring(L"filter*"), ctx.m_commands[0].m_name);
        }

        TEST_METHOD(RArgAndNothingElse_Parsed_ShowHelpAndError)
        {
            auto ctx = CallParse({ FirstArg(), L"-r" });

            Assert::AreEqual<size_t>(0, ctx.m_commands.size());
            Assert::IsTrue(ctx.m_showHelp);
            Assert::IsFalse(ctx.m_errorMessage.empty());
        }

        TEST_METHOD(RArgAndAnythingElse_Parsed_ShowRegexFiltered)
        {
            auto ctx = CallParse({ FirstArg(), L"-r", L"regexfilter.*" });

            Assert::AreEqual<size_t>(1, ctx.m_commands.size());
            Assert::AreEqual<int>((int)AppCommandType::RegexListEntries, (int)ctx.m_commands[0].m_commandType);
            Assert::AreEqual(std::wstring(L"regexfilter.*"), ctx.m_commands[0].m_name);
        }

    private:
        AppContext CallParse(std::initializer_list<wchar_t*> args)
        {
            CommandLineManager clm{};
            return clm.ParseCommandLine(args.size(), args.begin());
        }
    };
}