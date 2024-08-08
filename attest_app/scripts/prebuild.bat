@ECHO OFF
set prebuild_log_file="%~dp0prebuild.log"
call "%~dp0env.bat"
call "%provisioning_dir%SM/Helper/update_nsapp_setup.bat" %1 %2 > %prebuild_log_file% 2>&1
if %errorlevel% neq 0 (if %errorlevel% neq 2 (goto :error))
if %errorlevel% equ 2 echo        xml have been copied from %projectdir%Images

:: All the steps were executed correctly
echo        Successful application update according to flash layout
exit 0

:: Error when script is executed
:error
echo        Error, prebuild.bat script aborted, check prebuild.log
exit 1
