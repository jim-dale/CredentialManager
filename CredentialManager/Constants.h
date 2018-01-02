#pragma once

struct Constants
{
    static const std::wstring DefaultFilter;
};


// Definitions - we can do this in the header because there is only one cpp file
const std::wstring Constants::DefaultFilter(L"*");
