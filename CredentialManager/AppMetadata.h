#pragma once

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define VERSION(x) "2.1.0-" STRINGIFY(x)

static const PTCHAR ProgramName = _T("CredentialManager");
static const PTCHAR ProgramVersion = _T(VERSION(BUILDNUM));

#if defined(_DEBUG)
static const PTCHAR ProgramConfig = _T("Debug");
#elif defined(NDEBUG)
static const PTCHAR ProgramConfig = _T("Release");
#else
static const PTCHAR ProgramConfig = _T("Undefined");
#endif
#if defined(_M_IX86)
static const PTCHAR ProgramPlatform = _T("X86");
#elif defined(_M_X64)
static const PTCHAR ProgramPlatform = _T("X64");
#else
static const PTCHAR ProgramPlatform = _T("Undefined");
#endif
