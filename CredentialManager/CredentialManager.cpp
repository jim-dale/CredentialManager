//  CredentialManager
//    Command line interface to the Win32 Credential API
//

#include "includes.h"

int wmain(int argc, PWSTR argv[])
{
	CommandLineManager clm{};

	AppContext appContext = clm.ParseCommandLine(argc, argv);

	appContext.SetDefaults();

	if (appContext.m_showHelp)
	{
		clm.ShowHelp();
	}
	else if (appContext.m_showVersion)
	{
		clm.ShowVersion();
	}
	else if (appContext.m_showConfiguration)
	{
		clm.ShowConfiguration(appContext);
	}
	else
	{
		if (appContext.Initialise())
		{
			for (const auto& command : appContext.m_commands)
			{
				switch (command.m_commandType)
				{
				case AppCommandType::GetPassword:
					appContext.GetPassword(command);
					break;
				case AppCommandType::AddEntry:
					appContext.CreateEntry(command);
					break;
				default:
					break;
				}
			}
		}
		appContext.Uninitialise();
	}
	clm.ShowIfError(appContext);

	return appContext.m_exitCode;
}
