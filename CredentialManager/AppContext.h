#pragma once

struct AppContext
{
public:
    bool                m_showHelp;
    bool                m_showVersion;
    bool                m_showConfiguration;
    std::wstring        m_prefix;
    std::wstring        m_outputFileName;
    AppCommands         m_commands;

    FILE*               m_outputFile;
    Win32CredentialManager   m_credentialManager;
    std::wstring        m_errorMessage;
    int                 m_exitCode;

    void Configure()
    {
        m_prefix = Utility::GetPrefix(m_prefix);
        m_outputFile = stdout;
    }

    bool Initialise()
    {
        bool result = true;

        if (false == m_outputFileName.empty())
        {
            FILE* fd = nullptr;
            errno_t err = _wfopen_s(&fd, m_outputFileName.c_str(), L"w");
            if (0 == err && fd != nullptr)
            {
                (void)_setmode(_fileno(fd), _O_U8TEXT);
                m_outputFile = fd;
            }
        }

        return result;
    }

    void Uninitialise()
    {
        if (m_outputFile != stdout && m_outputFile != nullptr)
        {
            fclose(m_outputFile);
            m_outputFile = nullptr;
        }
    }

    void GetPassword(const AppCommand& command)
    {
        std::wstring name = Win32CredentialManager::BuildFullTargetName(m_prefix, command.m_name);

        auto credential = m_credentialManager.GetEntry(name);
        if (credential.Empty() == false)
        {
            fwprintf(m_outputFile, L"%s\n", credential.m_password.c_str());
        }
        else
        {
            m_exitCode = 1;
        }
    }

    void CreateEntry(const AppCommand& command)
    {
        std::wstring name = Win32CredentialManager::BuildFullTargetName(m_prefix, command.m_name);

        m_credentialManager.AddEntry(name, command.m_password);
    }

    void ListEntries(const AppCommand& command)
    {
        std::wstring filter = Win32CredentialManager::BuildFullTargetName(m_prefix, command.m_name);

        auto items = m_credentialManager.GetEntries(filter);
        std::sort(items.begin(), items.end(), SimpleGenericCredential::CompareByName);

        for (auto item : items)
        {
            fwprintf(m_outputFile, L"\"%s\",\"%s\"\n", item.m_name.c_str(), item.m_password.c_str());
        }
    }

    void RegexListEntries(const AppCommand& command)
    {
        std::wstring regexExpression(L"^");
        regexExpression.append(m_prefix);
        regexExpression.append(command.m_name);

        auto items = GetRegexEntries(regexExpression);
        std::sort(items.begin(), items.end(), SimpleGenericCredential::CompareByName);

        for (auto item : items)
        {
            fwprintf(m_outputFile, L"\"%s\",\"%s\"\n", item.m_name.c_str(), item.m_password.c_str());
        }
    }

private:
    CredentialCollection GetRegexEntries(const std::wstring& regexExpression)
    {
        CredentialCollection result;

        auto regex = std::wregex(regexExpression);

        auto items = m_credentialManager.GetAllEntries();
        for (auto item : items)
        {
            std::wstring toMatch(item.m_name);

            if (std::regex_match(toMatch, regex))
            {
                result.push_back(item);
            }
        }
        return result;
    }
};
