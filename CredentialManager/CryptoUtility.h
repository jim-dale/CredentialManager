#pragma once

class CryptoUtility
{
public:
	static std::vector<BYTE> EncryptToWstring(const std::wstring& value)
	{
		std::vector<BYTE> result;

		DATA_BLOB dataIn{};
		dataIn.pbData = (LPBYTE)value.data();
		dataIn.cbData = (DWORD)(value.length() * sizeof(std::wstring::value_type));

		DATA_BLOB dataOut;

		if (CryptProtectData(&dataIn, nullptr, nullptr, nullptr, nullptr, 0, &dataOut))
		{
			result.assign(dataOut.pbData, dataOut.pbData + dataOut.cbData);
		}

		return result;
	}

	static std::wstring DecryptAsWString(const std::vector<BYTE>& value)
	{
		std::wstring result;

		DATA_BLOB dataIn{};
		dataIn.pbData = (LPBYTE)value.data();
		dataIn.cbData = (DWORD)value.size();

		DATA_BLOB dataOut;

		if (CryptUnprotectData(&dataIn, nullptr, nullptr, nullptr, nullptr, 0, &dataOut))
		{
			auto count = dataOut.cbData / sizeof(std::wstring::value_type);
			if (count > 0)
			{
				result.assign((std::wstring::const_pointer)dataOut.pbData, count);
			}
		}

		return result;
	}
};

