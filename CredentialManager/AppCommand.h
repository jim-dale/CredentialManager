#pragma once

struct AppCommand
{
    AppCommandType  m_commandType;
    std::wstring    m_name;
    std::wstring    m_password;

    static AppCommand CreateListCommand(const std::wstring& filter)
    {
        AppCommand result{};

        result.m_commandType = AppCommandType::ListEntries;
        if (filter.empty())
        {
            result.m_name = L"*";
        }
        else
        {
            result.m_name = filter;
        }
        return result;
    }

    static AppCommand CreateGetPasswordCommand(const std::wstring& name)
    {
        AppCommand result{};

        result.m_commandType = AppCommandType::GetPassword;
        result.m_name = name;

        return result;
    }

    static AppCommand CreateAddEntryCommand(const std::wstring& name, const std::wstring& password)
    {
        AppCommand result{};

        result.m_commandType = AppCommandType::AddEntry;
        result.m_name = name;
        result.m_password = password;

        return result;
    }
};

typedef std::vector<AppCommand> AppCommands;

