#pragma once

class CommandLineManager
{
private:
    enum class ParserState
    {
        ExpectOption
        , ExpectGetName
        , ExpectAddEntryName
        , ExpectAddEntryPassword
        , ExpectPrefix
        , ExpectOutputFileName
    };

public:
    CommandLineManager()
    {
    }
    ~CommandLineManager()
    {
    }

    AppContext ParseCommandLine(size_t argc, wchar_t const* const* argv)
    {
        AppContext result{};

        ParserState state = ParserState::ExpectOption;
        std::wstring value;

        result.m_showHelp = (argc == 1);
        result.m_encryptPassword = true;

        // Skip the first argument because by convention, argv[0] is the command with which the program is invoked.
        for (size_t i = 1; i < argc; i++)
        {
            PCWSTR arg = argv[i];
            size_t length = wcslen(arg);

            switch (state)
            {
            case ParserState::ExpectOption:
                if (length > 1 && (arg[0] == '-' || arg[0] == '/'))
                {
                    state = ProcessOption(result, &arg[1]);
                }
                break;
            case ParserState::ExpectGetName:
                result.m_commands.push_back(AppCommand::CreateGetPasswordCommand(arg));
                state = ParserState::ExpectOption;
                break;
            case ParserState::ExpectAddEntryName:
                value = arg;
                state = ParserState::ExpectAddEntryPassword;
                break;
            case ParserState::ExpectAddEntryPassword:
                result.m_commands.push_back(AppCommand::CreateAddEntryCommand(value, arg));
                state = ParserState::ExpectOption;
                break;
            case ParserState::ExpectPrefix:
                result.m_prefix = arg;
                state = ParserState::ExpectOption;
                break;
            case ParserState::ExpectOutputFileName:
                result.m_outputFileName = arg;
                state = ParserState::ExpectOption;
                break;
            default:
                break;
            }
        }
        return result;
    }

    void ShowHelp()
    {
        ShowVersion();

        wprintf(L"Manages credentials in the Windows Credential Manager store.\n\n");

        wprintf(L"Usage: %s [-?] [-v] [-c] [-p prefix] [-o filename] [-s name password] [-g Name] [-c[-]]\n\n", ProgramName);

        wprintf(L"  -?  Display this help information.\n");
        wprintf(L"  -v  Display version information.\n");
        wprintf(L"  -c  Display the runtime configuration and exit.\n");
        wprintf(L"  -p  Sets the prefix for the credential. The prefix is combined with the name to produce a unique name.\n");
        wprintf(L"  -o  Set output filename.\n");
        wprintf(L"  -s  Sets the password for the given credential name. Overwrites the existing password if the name already exists.\n");
        wprintf(L"  -g  Display the password for the named generic credential.\n");
        wprintf(L"  -e  Use encryption to store the password in the Windows Credential Store. Encryption is enabled by default.\n");
    }

    void ShowVersion()
    {
        std::wstring notSet(Check_SourceVersion);

        std::wstring gitHash(GitHash);
        if (gitHash.compare(notSet) != 0 && gitHash.length() > notSet.length())
        {
            gitHash = gitHash.substr(0, gitHash.length() - notSet.length());
        }

        std::wstring shortGitHash(gitHash);
        if (shortGitHash.length() > SHORTGITHASHLEN)
        {
            shortGitHash = shortGitHash.substr(shortGitHash.length() - SHORTGITHASHLEN);
        }

        wprintf(L"%s %s-%s-%s-%s (%s)\n", ProgramName, ProgramVersion, shortGitHash.c_str(), ProgramPlatform, ProgramConfig, gitHash.c_str());
    }

    void ShowIfError(const AppContext& ctx)
    {
        if (false == ctx.m_errorMessage.empty())
        {
            fwprintf(stderr, L"\n%s\n", ctx.m_errorMessage.c_str());
        }
    }

    void ShowConfiguration(const AppContext& ctx)
    {
        wprintf(L"ShowHelp: %s\n", Utility::to_wstr(ctx.m_showHelp));
        wprintf(L"ShowVersion: %s\n", Utility::to_wstr(ctx.m_showVersion));
        wprintf(L"ShowConfiguration: %s\n", Utility::to_wstr(ctx.m_showConfiguration));
        wprintf(L"Prefix: \"%s\"\n", ctx.m_prefix.c_str());
        wprintf(L"OutputFileName: \"%s\"\n", ctx.m_outputFileName.c_str());
        wprintf(L"UseEncryption: %s\n", Utility::to_wstr(ctx.m_encryptPassword));
        wprintf(L"Commands:\n");
        for (auto& command : ctx.m_commands)
        {
            wprintf(L"    CommandType: \"%s\"\n", Utility::to_wstr(command.m_commandType));
            wprintf(L"    Name: \"%s\"\n", command.m_name.c_str());
            wprintf(L"    Password: \"%s\"\n", command.m_password.c_str());
        }
        wprintf(L"\n");
    }

private:
    ParserState ProcessOption(AppContext& appContext, PCWSTR arg)
    {
        ParserState result = ParserState::ExpectOption;

        if (_wcsicmp(L"?", arg) == 0)
        {
            appContext.m_showHelp = true;
        }
        else if (_wcsicmp(L"v", arg) == 0)
        {
            appContext.m_showVersion = true;
        }
        else if (_wcsicmp(L"c", arg) == 0)
        {
            appContext.m_showConfiguration = true;
        }
        else if (_wcsicmp(L"g", arg) == 0)
        {
            result = ParserState::ExpectGetName;
        }
        else if (_wcsicmp(L"o", arg) == 0)
        {
            result = ParserState::ExpectOutputFileName;
        }
        else if (_wcsicmp(L"p", arg) == 0)
        {
            result = ParserState::ExpectPrefix;
        }
        else if (_wcsicmp(L"s", arg) == 0)
        {
            result = ParserState::ExpectAddEntryName;
        }
        else if (_wcsicmp(L"e", arg) == 0)
        {
            appContext.m_encryptPassword = true;
        }
        else if (_wcsicmp(L"e-", arg) == 0)
        {
            appContext.m_encryptPassword = false;
        }
        else
        {
            appContext.m_showHelp = true;
            appContext.m_errorMessage = std::wstring(L"Parameter format not correct - '") + std::wstring(arg) + std::wstring(L"'.");
        }
        return result;
    }
};
