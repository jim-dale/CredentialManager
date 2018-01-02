#pragma once

struct SimpleGenericCredential
{
    std::wstring m_name;
    std::wstring m_password;

    bool Empty()
    {
        return m_name.empty() && m_password.empty();
    }

    static bool CompareByName(const SimpleGenericCredential& lhs, const SimpleGenericCredential& rhs)
    {
        return lhs.m_name < rhs.m_name;
    }
};

typedef std::vector<SimpleGenericCredential> CredentialCollection;
