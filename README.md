# CredentialManager
Command line utility to manage credentials in the Windows Credential Manager store

The goal is to automate the creation of encrypted archives for offline storage without embedding passwords in script files.
The example below creates a backup of the current user's profile directory in an encrypted zip file. 

```dosbatch
@SETLOCAL

@FOR /F "tokens=*" %%I IN ('gettimestamp') DO @SET TIMESTAMP=%%I
@FOR /F "tokens=*" %%I IN ('CredentialManager -g MyPasswordTargetName') DO @SET PASSWORD=%%I

@SET ZIP=7za.exe
@SET ZIPOPTS=-tzip -mem=AES256 -p"%PASSWORD%"

@%ZIP% a %ZIPOPTS% "%TIMESTAMP%-%USERNAME%.enc.zip" "%USERPROFILE%\*"

@ENDLOCAL
```

Notes
* The password must have previously been stored in Windows Credential Manager with the name 'MyPasswordTargetName'.
* The 7-Zip command line executable `7za.exe` must be on the PATH.
