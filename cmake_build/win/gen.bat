@echo OFF
rem IMPORTANT Call this batch from eos_client/build/ directory

if "%1"=="" goto usage

rem Версия используемой VisualStudio
set VSVER=%1

set PLATFORM=amd64
if /i "%2"=="x64"       set PLATFORM=amd64
if /i "%2"=="x86"       set PLATFORM=x86

rem Платформа, используется при генерации проекта MSVS
set PROJ_TYPE=x64
if /i %PLATFORM%==x86       set PROJ_TYPE=Win32

rem Платформа (x86/x64), используется в CMakeLists.txt для определения папки с библиотеками
set LIB_PLATFORM_PARAM=x64
if /i %PLATFORM%==x86 set LIB_PLATFORM_PARAM=x86

rem Очищаем текущий каталог для генерации новых файлов проекта
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)

rem Генерируем файлы решения VisualStudio
cmake %~dp0 -DLIB_PLATFORM=%LIB_PLATFORM_PARAM% -G "Visual Studio %VSVER%" -A %PROJ_TYPE%

goto end

:usage
echo:
echo Error in script usage. The correct usage is:
echo     %0 [version] [platform]
echo where [version] is version of installed Visual Studio, e.g. 11 is VisualStudio 2012 and 12 is VisualStudio 2013
echo       [platform] is [x86 ^| x64 ^| arm ^| x86_arm ^| x86_amd64] (optional, default is x86)
echo:
echo For example:
echo     %0 11 x64
goto :end

:end