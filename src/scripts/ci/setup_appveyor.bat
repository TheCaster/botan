
echo Current build setup MSVS="%MSVS%" PLATFORM="%PLATFORM%" TARGET="%TARGET%"

if %MSVS% == 2013 call "%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" %PLATFORM%
if %MSVS% == 2015 call "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %PLATFORM%
if %MSVS% == 2017 call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %PLATFORM%

rem check compiler version
cl

appveyor DownloadFile http://download.qt.io/official_releases/jom/jom.zip -FileName jom.zip
7z e jom.zip

nuget install -ExcludeVersion -Version 4.1.0 clcache

set PATH=%PATH%;C:\projects\botan\clcache\clcache-4.1.0

rem AppVeyor has a hard 1Gb cache limit for OSS builds
clcache -M 157286400
