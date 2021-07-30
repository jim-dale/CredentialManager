#pragma once

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

        TEST_METHOD(EArg_Parsed_SetEncryptionTrue)
        {
            auto ctx = CallParse({ FirstArg(), L"-e" });

            Assert::IsTrue(ctx.m_encryptPassword);
        }

        TEST_METHOD(NegatedEArg_Parsed_SetEncryptionTrue)
        {
            auto ctx = CallParse({ FirstArg(), L"-e-" });

            Assert::IsFalse(ctx.m_encryptPassword);
        }

        TEST_METHOD(GArg_Parsed_GetPasswordCommandCreated)
        {
            auto ctx = CallParse({ FirstArg(), L"-g", L"Name" });

            Assert::AreEqual<size_t>(1, ctx.m_commands.size());
            Assert::AreEqual<int>((int)AppCommandType::GetPassword, (int)ctx.m_commands[0].m_commandType);
            Assert::AreEqual(std::wstring(L"Name"), ctx.m_commands[0].m_name);
        }

        TEST_METHOD(SArg_Parsed_SetPasswordCommandCreated)
        {
            auto ctx = CallParse({ FirstArg(), L"-s", L"Name", L"Password" });

            Assert::AreEqual<size_t>(1, ctx.m_commands.size());
            Assert::AreEqual<int>((int)AppCommandType::AddEntry, (int)ctx.m_commands[0].m_commandType);
            Assert::AreEqual(std::wstring(L"Name"), ctx.m_commands[0].m_name);
            Assert::AreEqual(std::wstring(L"Password"), ctx.m_commands[0].m_password);
        }

    private:
        AppContext CallParse(std::initializer_list<wchar_t*> args)
        {
            CommandLineManager clm{};
            return clm.ParseCommandLine(args.size(), args.begin());
        }
    };
}
