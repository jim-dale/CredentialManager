# CredentialManager
Command line utility to manage credentials in the Windows Credential Manager store

## Building the code
To build the code you must define the `BUILD_BUILDNUMBER` environment variable otherwise the build will fail.

## Usage
```dosbatch
CredentialManager [-?] [-v] [-c] [-l filter] [-r regex] [-p prefix] [-o filename] [-s name password] [-g Name]

  -?  Display this help information.
  -v  Display version information.
  -c  Display the runtime configuration.
  -l  List entries with the specified filter. The filter is appended to the prefix to match entries.
      The default filter is '*' to list all entries.
  -r  List entries with the specified regular expression filter. The filter is appended to the prefix to match entries.
  -o  Set output filename.
  -p  Sets the prefix for the credential. The prefix is combined with the name to produce a unique name.
  -g  Display the password for the named generic credential.
  -s  Sets the password for the given credential name. Overwrites the existing password if the name already exists.
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
