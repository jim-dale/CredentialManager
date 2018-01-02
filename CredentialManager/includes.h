#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <wincred.h>

#include <stdio.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>

#include <algorithm>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "Constants.h"
#include "AppCommandType.h"
#include "Utility.h"
#include "AppMetadata.h"
#include "SimpleGenericCredential.h"
#include "Win32CredentialManager.h"

#include "AppCommand.h"
#include "AppContext.h"
#include "CommandLineManager.h"
