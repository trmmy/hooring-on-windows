@ECHO OFF
REM
REM download source and ruby, and rake source
REM

SET PWD=%CD%

IF EXIST .git (
  FOR /F "delims=	" %%A IN (.git\config) DO (
    IF "%%A" == "url = https://github.com/trmmy/hooring-on-windows.git" (
      echo found hooring-on-windows' repository - %CD%\.git
      GOTO WORKSPACE
    )
    IF "%%A" == "url = https://github.com/trmmy/hooring-on-windows" (
      echo found hooring-on-windows' repository - %CD%\.git
      GOTO WORKSPACE
    )
  )
)

FOR /D %%A IN (*) DO (
  ECHO found some folders
  GOTO NOT_EMPTY
)
FOR %%A IN (*) DO (
  IF NOT "%%A" == "%~nx0" (
    ECHO found some files
    GOTO NOT_EMPTY
  )
)

GOTO WORKSPACE
:NOT_EMPTY

IF NOT EXIST hooring-on-windows (
  ECHO mkdir -p %CD%\hooring-on-windows
  MKDIR hooring-on-windows
) 
ECHO move to hooring-on-windows
CD hooring-on-windows

:WORKSPACE

IF NOT EXIST tools (
  MKDIR tools
)
CD tools

SET tools=%CD%
SET cache=%tools%\cache
IF NOT EXIST %cache% (
  ECHO mkdir -p %cache%
  MKDIR %cache% 2> NUL
)

SET ps=%SystemRoot%\system32\WindowsPowerShell\v1.0\powershell.exe

REM
REM 7zip
REM
REM   to inflate curl, ruby, git
REM
SET sevenzip=1600
PATH=%tools%\7z%sevenzip%-x64\Files\7-zip;%PATH%
IF NOT EXIST %tools%\7z%sevenzip%-x64\Files\7-zip\7z.exe (
  ECHO not found %tools%\7z%sevenzip%-x64\Files\7-zip\7z.exe
  IF NOT EXIST %cache%\7z%sevenzip%-x64.msi (
    ECHO not found %cache%\7z%sevenzip%-x64.msi
    ECHO download http://7-zip.org/a/7z%sevenzip%-x64.msi ^> %cache%/7z%sevenzip%-x64.msi
    %ps% Invoke-WebRequest -Uri "http://7-zip.org/a/7z%sevenzip%-x64.msi" -Outfile %cache%\7z%sevenzip%-x64.msi
  )
  ECHO inflate %cache%\7z%sevenzip%-x64.msi ^> %tools%\7z%sevenzip%-x64
  IF NOT EXIST %tools%\7z%sevenzip%-x64 (
    MKDIR %tools%\7z%sevenzip%-x64
  )
  msiexec.exe /A %cache%\7z%sevenzip%-x64.msi targetdir="%tools%\7z%sevenzip%-x64" /QN /LOG log
)

REM
REM curl
REM
REM   to download ruby, git
REM
SET curl=curl_748_0_rtmp_ssh2_ssl_sspi
PATH=%tools%\%curl%;%PATH%
IF NOT EXIST %tools%\%curl%\curl.exe (
  IF NOT EXIST %cache%\%curl%.zip (
    ECHO download http://www.paehl.com/open_source/?download=%curl%.zip ^> %cache%\%curl%.zip
    %ps% Invoke-WebRequest -Uri "http://www.paehl.com/open_source/?download=%curl%.zip" -Outfile %cache%\%curl%.zip
  )
  ECHO inflate %cache%\%curl%.zip ^> %tools%\%curl%
  7z.exe x %cache%\%curl%.zip -o%tools%\%curl% > NUL
)

REM cacert.pem
IF NOT EXIST cacert.pem (
  ECHO download https://curl.haxx.se/ca/cacert.pem ^> %tools%\cacert.pem
  curl.exe -s -k -L "https://curl.haxx.se/ca/cacert.pem" > %tools%\cacert.pem
)

REM
REM ruby
REM
REM   to rake this application
REM
SET ruby=ruby-2.3.0-x64-mingw32
PATH=%tools%\%ruby%\bin;%PATH%
IF NOT EXIST %tools%\%ruby%\bin\ruby.exe (
  ECHO not found %tools%\%ruby%\bin\ruby.exe
  IF NOT EXIST %cache%\%ruby%.7z (
    ECHO not found %cache%\%ruby%.7z
    ECHO download http://dl.bintray.com/oneclick/rubyinstaller/%ruby%.7z ^> %cache%\%ruby%.7z
    curl.exe -s --cacert %tools%\cacert.pem -L http://dl.bintray.com/oneclick/rubyinstaller/%ruby%.7z > %cache%\%ruby%.7z
  )
  ECHO inflate %cache%\%ruby%.7z
  7z.exe x %cache%\%ruby%.7z > NUL
)

REM
REM git
REM
REM   to clone or update this application
REM
SET git_version=2.8.2
SET git=PortableGit-%git_version%-64-bit
PATH=%PATH%;%tools%\%git%\bin
IF NOT EXIST %tools%\%git%\bin\git.exe (
  IF NOT EXIST %cache%\%git%.7z.exe (
    ECHO download https://github.com/git-for-windows/git/releases/download/v%git_version%.windows.1/%git%.7z.exe ^> %cache%\%git%.7z.exe
    curl.exe -s --cacert %tools%\cacert.pem -L https://github.com/git-for-windows/git/releases/download/v%git_version%.windows.1/%git%.7z.exe > %cache%\%git%.7z.exe
  )
  ECHO inflate %cache%\%git%.7z.exe ^> %tools%\%git%
  7z.exe x %cache%\%git%.7z.exe -o%tools%\%git% > NUL
)

CD ..

REM
REM source
REM
IF NOT EXIST .git (
  git init
  git remote add origin https://github.com/trmmy/hooring-on-windows.git
  git remote update
  git checkout origin/master -f
)
git submodule init
git submodule update

CALL rake.bat

CD %PWD%
