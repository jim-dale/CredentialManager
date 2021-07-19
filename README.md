# CredentialManager
Command line utility to manage credentials in the Windows Credential Manager store

## Usage
```dosbatch
CredentialManager [-?] [-v] [-c] [-p prefix] [-o filename] [-s name password] [-g Name] [-c[-]]

  -?  Display this help information.
  -v  Display version information.
  -c  Display the runtime configuration and exit.
  -p  Sets the prefix for the credential. The prefix is combined with the name to produce a unique name.
  -o  Set output filename.
  -s  Sets the password for the given credential name. Overwrites the existing password if the name already exists.
  -g  Display the password for the named generic credential.
  -e  Use encryption to store the password in the Windows Credential Store. Encryption is enabled by default.
```

The overall aim is to automate the creation of encrypted archives for offline storage without embedding passwords in scripts.
The example below creates a backup of the current user's profile directory in an encrypted zip file. 


```dosbatch
@SETLOCAL

@FOR /F "tokens=*" %%I IN ('gettimestamp') DO @SET TIMESTAMP=%%I
@FOR /F "tokens=*" %%I IN ('CredentialManager -g MyPasswordTargetName') DO @SET PASSWORD=%%I

@SET ZIP=7za.exe
@SET ZIPOPTS=-r -tzip -stl -mem=AES256 -p"%PASSWORD%"

@%ZIP% a %ZIPOPTS% "%TIMESTAMP%-%USERNAME%.enc.zip" "%USERPROFILE%\*"

@ENDLOCAL
```

Notes
* The password must have previously been stored in Windows Credential Manager with the name 'MyPasswordTargetName'.
* The 7-Zip command line executable `7za.exe` must be on the PATH.
