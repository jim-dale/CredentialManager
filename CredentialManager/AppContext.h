#pragma once

struct AppContext
{
public:
	bool                m_showHelp;
	bool                m_showVersion;
	bool                m_showConfiguration;
	std::wstring        m_prefix;
	std::wstring        m_outputFileName;
	bool				m_encryptPassword;
	AppCommands         m_commands;

	FILE*				m_outputFile;
	std::shared_ptr<IWin32CredentialConverter> m_credentialConverter;
	std::unique_ptr<Win32CredentialManager> m_credentialManager;
	std::wstring        m_errorMessage;
	int                 m_exitCode;

	void SetDefaults()
	{
		m_prefix = Utility::GetPrefix(m_prefix);
		m_outputFile = stdout;
	}

	bool Initialise()
	{
		bool result = true;

		if (false == m_outputFileName.empty())
		{
			FILE* fd = nullptr;
			errno_t err = _wfopen_s(&fd, m_outputFileName.c_str(), L"w");
			if (0 == err && fd != nullptr)
			{
				(void)_setmode(_fileno(fd), _O_U8TEXT);
				m_outputFile = fd;
			}
		}
		if (m_encryptPassword)
		{
			m_credentialConverter = std::make_shared<CryptoCredentialConverter>();
		}
		else
		{
			m_credentialConverter = std::make_shared<PlaintextCredentialConverter>();
		}
		m_credentialManager = std::make_unique<Win32CredentialManager>(m_credentialConverter);

		return result;
	}

	void Uninitialise()
	{
		if (m_outputFile != stdout && m_outputFile != nullptr)
		{
			fclose(m_outputFile);
			m_outputFile = nullptr;
		}
	}

	void GetPassword(const AppCommand& command)
	{
		std::wstring name = Utility::Concat(m_prefix, command.m_name);

		auto credential = m_credentialManager->GetEntry(name);
		if (credential.Empty() == false)
		{
			fwprintf(m_outputFile, L"%s\n", credential.m_password.c_str());
		}
		else
		{
			m_exitCode = 1;
		}
	}

	void CreateEntry(const AppCommand& command)
	{
		std::wstring name = Utility::Concat(m_prefix, command.m_name);

		m_credentialManager->AddEntry(name, command.m_password);
	}
};
