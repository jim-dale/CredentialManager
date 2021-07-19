#pragma once

class Win32CredentialManager
{
public:
	HRESULT hres;

	Win32CredentialManager(std::shared_ptr<IWin32CredentialConverter> converter)
		: m_converter(converter), hres(S_OK)
	{
	}

	bool Exists(const std::wstring& name)
	{
		bool result = false;

		PCREDENTIAL pCredential = nullptr;
		BOOL success = ::CredRead(name.c_str(), CRED_TYPE_GENERIC, 0, &pCredential);
		if (FALSE != success)
		{
			result = true;
			::CredFree(pCredential);
		}
		return result;
	}

	void AddEntry(const std::wstring& name, const std::wstring& password)
	{
		auto item = m_converter->ConvertTo(name, password);

		CREDENTIAL credential{};
		credential.Type = CRED_TYPE_GENERIC;
		credential.Persist = CRED_PERSIST_LOCAL_MACHINE;
		credential.TargetName = (LPWSTR)item.m_name.c_str();
		credential.CredentialBlobSize = (DWORD)item.m_blob.size();
		credential.CredentialBlob = (PBYTE)item.m_blob.data();

		BOOL success = ::CredWrite(&credential, 0);
		if (FALSE == success)
		{
			SetHResult();
		}
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
			result = m_converter->ConvertFrom(pCredential->TargetName, pCredential->CredentialBlob, pCredential->CredentialBlobSize);

			::CredFree(pCredential);
		}
		return result;
	}

	void DeleteEntry(const std::wstring& name)
	{
		BOOL success = ::CredDelete(name.c_str(), CRED_TYPE_GENERIC, 0);
		if (FALSE == success)
		{
			SetHResult();
		}
	}

private:
	std::shared_ptr<IWin32CredentialConverter> m_converter;

	void SetHResult()
	{
		DWORD err = ::GetLastError();
		hres = HRESULT_FROM_WIN32(err);
	}
};
