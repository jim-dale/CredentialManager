#pragma once

class Win32CredentialManager
{
public:
    HRESULT hres;

    bool Exists(const std::wstring& name)
    {
        bool result = false;

        PCREDENTIAL pCredential = nullptr;
        BOOL success = ::CredRead(name.c_str(), CRED_TYPE_GENERIC, 0, &pCredential);
        if (FALSE == success)
        {
            SetHResult();
        }
        else
        {
            result = true;
            ::CredFree(pCredential);
        }
        return result;
    }

    CredentialCollection GetEntries(const std::wstring& filter)
    {
        CredentialCollection result;

        DWORD count = 0;
        PCREDENTIAL* pCredentials = nullptr;
        BOOL success = ::CredEnumerate(filter.c_str(), 0, &count, &pCredentials);
        if (FALSE == success)
        {
            SetHResult();
        }
        else
        {
            for (DWORD i = 0; i < count; i++)
            {
                PCREDENTIAL pCredential = pCredentials[i];

                std::wstring name(pCredential->TargetName);
                std::wstring password = Win32CredentialManager::ConvertBlobToWString(pCredential);

                SimpleGenericCredential item
                {
                    name,
                    password
                };

                result.push_back(item);
            }
            ::CredFree(pCredentials);
        }
        return result;
    }

    CredentialCollection GetAllEntries()
    {
        return GetEntries(Constants::DefaultFilter);
    }

    SimpleGenericCredential GetEntry(const std::wstring& name)
    {
        SimpleGenericCredential result{};

        PCREDENTIAL pCredential = nullptr;
        BOOL success = ::CredRead(name.c_str(), CRED_TYPE_GENERIC, 0, &pCredential);
        if (FALSE == success)
        {
            SetHResult();
        }
        else
        {
            result.m_name = name;
            result.m_password = ConvertBlobToWString(pCredential);

            ::CredFree(pCredential);
        }
        return result;
    }

    void AddEntry(const std::wstring& name, const std::wstring& password)
    {
        size_t size = (password.length() * sizeof(std::wstring::value_type));

        CREDENTIAL credential{};
        credential.Type = CRED_TYPE_GENERIC;
        credential.TargetName = (LPWSTR)name.c_str();
        credential.CredentialBlobSize = (DWORD)size;
        credential.CredentialBlob = (LPBYTE)password.c_str();
        credential.Persist = CRED_PERSIST_ENTERPRISE;

        BOOL success = ::CredWrite(&credential, 0);
        if (FALSE == success)
        {
            SetHResult();
        }
    }

    void DeleteEntry(const std::wstring& name)
    {
        BOOL success = ::CredDelete(name.c_str(), CRED_TYPE_GENERIC, 0);
        if (FALSE == success)
        {
            SetHResult();
        }
    }

    static std::wstring BuildFullTargetName(const std::wstring& prefix, const std::wstring& name)
    {
        return prefix + name;
    }

    static std::wstring ConvertBlobToWString(PCREDENTIAL pCredential)
    {
        size_t count = pCredential->CredentialBlobSize / sizeof(std::wstring::value_type);

        std::wstring result((std::wstring::const_pointer)pCredential->CredentialBlob, count);

        return result;
    }

private:
    void SetHResult()
    {
        DWORD err = ::GetLastError();
        hres = HRESULT_FROM_WIN32(err);
    }
};
