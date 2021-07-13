#pragma once

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define VERSION(x) "2.2.0-" STRINGIFY(x)

static const wchar_t* ProgramName = _T("CredentialManager");
static const wchar_t* ProgramVersion = _T(VERSION(BUILD_SOURCEVERSION));

#if defined(_DEBUG)
static const wchar_t* ProgramConfig = _T("Debug");
#elif defined(NDEBUG)
static const wchar_t* ProgramConfig = _T("Release");
#else
static const wchar_t* ProgramConfig = _T("Undefined");
#endif
#if defined(_M_IX86)
static const wchar_t* ProgramPlatform = _T("X86");
#elif defined(_M_X64)
static const wchar_t* ProgramPlatform = _T("X64");
#else
static const wchar_t* ProgramPlatform = _T("Undefined");
#endif
