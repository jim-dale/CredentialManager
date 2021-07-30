#pragma once

struct SimpleGenericCredential
{
    std::wstring m_name;
    blob m_blob;
    std::wstring m_password;

    bool Empty()
    {
        return m_name.empty();
    }

    static bool CompareByName(const SimpleGenericCredential& lhs, const SimpleGenericCredential& rhs)
    {
        return lhs.m_name < rhs.m_name;
    }
};
