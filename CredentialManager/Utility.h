#pragma once

class Utility
{
public:
    static std::wstring GetPrefix(const std::wstring& prefix)
    {
        std::wstring result;

        std::wstring separator(L"_");
        const int BufferSize = 512;
        wchar_t buffer[BufferSize];

        if (false == prefix.empty())
        {
            DWORD length = ExpandEnvironmentStrings(prefix.c_str(), buffer, BufferSize);
            // length includes nul terminator character
            if (length > 1 && length < BufferSize)
            {
                result.assign(buffer);
            }
        }

        if (false == result.empty() && false == ends_with(result, separator))
        {
            result.append(separator);
        }
        return result;
    }

    static std::wstring Concat(const std::wstring& prefix, const std::wstring& name)
    {
        return prefix + name;
    }

    static bool ends_with(const std::wstring& value, const std::wstring& ending)
    {
        bool result = false;

        size_t value_len = value.length();
        size_t end_len = ending.length();
        size_t offset = value_len - end_len;
        if (offset <= value_len)
        {
            result = (0 == value.compare(offset, end_len, ending));
        }
        return result;
    }

    static std::wstring ConvertToWString(const std::vector<BYTE>& value)
    {
        return ConvertBlobToWString(value.data(), value.size());
    }

    static std::wstring ConvertBlobToWString(BYTE const* const blob, size_t blobSize)
    {
        std::wstring result;

        size_t count = blobSize / sizeof(std::wstring::value_type);
        if (count > 0)
        {
            result.assign((std::wstring::const_pointer)blob, count);
        }
        return result;
    }

    static std::vector<BYTE> ConvertWstring(const std::wstring& value)
    {
        std::vector<BYTE> result;

        size_t count = (value.length() * sizeof(std::wstring::value_type));
        if (count > 0)
        {
            result.assign((BYTE*)value.data(), (BYTE*)(value.data() + count));
        }
        return result;
    }

    static wchar_t const*const to_wstr(bool b)
    {
        return b ? L"true" : L"false";
    }

    static wchar_t const*const to_wstr(AppCommandType commandType)
    {
        const wchar_t* result = L"";

        switch (commandType)
        {
        case AppCommandType::GetPassword:
            result = L"GetPassword";
            break;
        case AppCommandType::AddEntry:
            result = L"AddEntry";
            break;
        default:
            break;
        }
        return result;
    }
};
