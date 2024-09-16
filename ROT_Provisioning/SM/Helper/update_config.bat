:: Getting the Trusted Package Creator and STM32CubeProgammer CLI path
@echo off
call "%~dp0../../env.bat"

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Environment variable for log file
set "projectdir=%~dp0"

:start
goto exe:
goto py:
:exe
:: called if we want to use AppliCfg executable
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\dist\AppliCfg.exe"
set "python="
if exist %applicfg% (
goto update
)
:py
:: called if we just want to use AppliCfg python (think to comment "goto exe:")
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\AppliCfg.py"
set "python=python "

:update

:: Flash layout files
set flash_layout_bin="%projectdir%..\Binary\FlashLayoutConfigs.bin"
set general_cfg_file="%projectdir%..\Config\SM_Config_General.xml"


:: Get effective flash layout
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 0  -nxml_el_idx "Flash Layout configuration index" -n "Nb Secure Modules" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 4  -nxml_el_idx "Flash Layout configuration index" -n "S Module 0 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 5  -nxml_el_idx "Flash Layout configuration index" -n "S Module 0 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 6  -nxml_el_idx "Flash Layout configuration index" -n "S Module 0 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 7  -nxml_el_idx "Flash Layout configuration index" -n "S Module 1 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 8  -nxml_el_idx "Flash Layout configuration index" -n "S Module 1 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 9  -nxml_el_idx "Flash Layout configuration index" -n "S Module 1 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 10 -nxml_el_idx "Flash Layout configuration index" -n "S Module 2 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 11 -nxml_el_idx "Flash Layout configuration index" -n "S Module 2 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 12 -nxml_el_idx "Flash Layout configuration index" -n "S Module 2 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 13 -nxml_el_idx "Flash Layout configuration index" -n "S Module 3 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 14 -nxml_el_idx "Flash Layout configuration index" -n "S Module 3 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 15 -nxml_el_idx "Flash Layout configuration index" -n "S Module 3 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 16 -nxml_el_idx "Flash Layout configuration index" -n "S Module 4 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 17 -nxml_el_idx "Flash Layout configuration index" -n "S Module 4 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 18 -nxml_el_idx "Flash Layout configuration index" -n "S Module 4 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 19 -nxml_el_idx "Flash Layout configuration index" -n "S Module 5 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 20 -nxml_el_idx "Flash Layout configuration index" -n "S Module 5 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 21 -nxml_el_idx "Flash Layout configuration index" -n "S Module 5 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 22 -nxml_el_idx "Flash Layout configuration index" -n "S Module 6 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 23 -nxml_el_idx "Flash Layout configuration index" -n "S Module 6 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 24 -nxml_el_idx "Flash Layout configuration index" -n "S Module 6 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 25 -nxml_el_idx "Flash Layout configuration index" -n "S Module 7 primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 26 -nxml_el_idx "Flash Layout configuration index" -n "S Module 7 secondary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 27 -nxml_el_idx "Flash Layout configuration index" -n "S Module 7 size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 28 -nxml_el_idx "Flash Layout configuration index" -n "NS appli primary offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 29 -nxml_el_idx "Flash Layout configuration index" -n "NS appli secondary offset" -nxml "NS reserved area size" -e "val1+(val2/2)" -cond "val2>=0 and (val2 %% 2) == 0" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 30 -nxml_el_idx "Flash Layout configuration index" -n "NS appli size" -nxml "NS reserved area size" -e "val1-(val2/2)" -cond "val2>=0 and (val2 %% 2) == 0" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 31 -nxml_el_idx "Flash Layout configuration index" -n "NS reserved area offset" -nxml "NS appli primary offset" -nxml "NS appli size" -e "val2 + val3" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 33 -nxml_el_idx "Flash Layout configuration index" -n "ITS Factory offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 34 -nxml_el_idx "Flash Layout configuration index" -n "ITS Factory size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 35 -nxml_el_idx "Flash Layout configuration index" -n "ITS offset" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config
%AppliCfg% xmlval -b %flash_layout_bin% -b_el_idx 36 -nxml_el_idx "Flash Layout configuration index" -n "ITS size" %general_cfg_file% --vb
if !errorlevel! neq 0 goto :error_config


:: ============================================================= End functions ===================================================
:: All the steps were executed correctly

exit /b 0

:error_config
echo ===== Error occurred in Helper/update_config.bat script.
exit /b 1

