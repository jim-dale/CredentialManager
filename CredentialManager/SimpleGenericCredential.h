#pragma once

struct SimpleGenericCredential
{
    std::wstring m_name;
    std::wstring m_password;

    bool Empty()
    {
        return m_name.empty() && m_password.empty();
    }
};

typedef std::vector<SimpleGenericCredential> CredentialCollection;
