#pragma once

class IWin32CredentialConverter
{
public:
	IWin32CredentialConverter()
	{
	}
	virtual ~IWin32CredentialConverter()
	{
	}

	virtual SimpleGenericCredential ConvertTo(const std::wstring& name, const std::wstring& password) = 0;
	virtual SimpleGenericCredential ConvertFrom(LPWSTR name, PBYTE blob, DWORD blobSize) = 0;
};

class PlaintextCredentialConverter : public IWin32CredentialConverter
{
public:
	virtual SimpleGenericCredential ConvertTo(const std::wstring& name, const std::wstring& password) override
	{
		SimpleGenericCredential result{};

		result.m_name = name;
		result.m_blob = Utility::ConvertWstringToBlob(password);
		result.m_password = password;

		return result;
	}

	virtual SimpleGenericCredential ConvertFrom(LPWSTR name, PBYTE blob, DWORD blobSize) override
	{
		SimpleGenericCredential result{};

		result.m_name = name;
		result.m_blob.assign(blob, blob + blobSize);
		if (result.m_blob.empty() == false)
		{
			result.m_password = Utility::ConvertBlobToWString(result.m_blob);
		}
		return result;
	}
};

class CryptoCredentialConverter : public IWin32CredentialConverter
{
public:
	virtual SimpleGenericCredential ConvertTo(const std::wstring& name, const std::wstring& password) override
	{
		SimpleGenericCredential result{};

		result.m_name = name;
		result.m_blob = CryptoUtility::EncrypWstringToBlob(password);
		result.m_password = password;

		return result;
	}

	virtual SimpleGenericCredential ConvertFrom(LPWSTR name, PBYTE blob, DWORD blobSize) override
	{
		SimpleGenericCredential result{};

		result.m_name = name;
		result.m_blob.assign(blob, blob + blobSize);
		if (result.m_blob.empty() == false)
		{
			result.m_password = CryptoUtility::DecryptBlobToWString(result.m_blob);
		}
		return result;
	}
};
