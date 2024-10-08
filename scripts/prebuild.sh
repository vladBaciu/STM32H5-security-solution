#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

error_config()
{
  printf "${RED}"
  echo "prebuild script failed"
  echo "====="
  if [ -z "$1" ]; then
  echo "===== Error occurred."
  else
  echo "===== Error occurred. ($1)"
  fi
  echo "===== See $current_log_file for details. Then try again."
  echo "====="
  exit 1
}

success_config()
{
  rm "$current_log_file"
  printf "${GREEN}"
  echo "pre_build script success"
  echo

  # ============================================================= End functions ===================================================
  # All the steps were executed correctly
  printf "${NO}"
  exit 0
}
install_cube()
{
  echo "Please get latest version of STM32CubeProgrammer from st.com." 
  error_config "env"
}
install_package()
{
  echo "Please get STM32H5 Secure Manager package from st.com." |tee -a "$current_log_file"
  error_config "env"
}

##########################
#  pre_build.sh
##########################
# Environment variables for log files
thispath=$(cd "$(dirname "$0")" && pwd)
#unset cube_fw_path
current_log_file="$thispath/prebuild.log"
echo "Generated by sh"> "$current_log_file"
if [ "#$" == "2" ]; then
  echo "Usage $0 IAR/STM32CubeIDE/Kiel exec_file"|tee -a "$current_log_file"
  error_config "args"
fi
compil=$1
exedir="$2" #dir for exe to check the date
# Getting the Trusted Package Creator and STM32CubeProgammer CLI path
if [[ -z "$stm32programmercli" ]] || [[ -z "$stm32tpccli" ]] || [[ -z "$provisioning_dir" ]] || [[ -z "$cube_fw_path" ]] || [ -z "$sm_ns_app_boot_path_project" ]; then
  if [ -e "$thispath/env.sh" ]; then
    source "$thispath/env.sh" 2>/dev/null
  else
    echo "env.sh missing"
    install_package
  fi
fi


applicfg_path="$provisioning_dir"/../../../Utilities/PC_Software/ROT_AppliConfig/

uname | grep -i -e windows -e mingw
if [ $? == 0 ]; then
  #line for window executable
  if [ -e "$applicfg_path/dist/AppliCfg.exe" ]; then
    echo AppliCfg with windows executable
    AppliCfg="$applicfg_path/dist/AppliCfg.exe"
  fi
fi
if [[ -z "$AppliCfg" ]]; then
  #line for python
  if [ -e "$applicfg_path/AppliCfg.py" ]; then
    echo AppliCfg with python script
      #determine/check python version command
      [ ${-/x} != ${-} ] && AppliCfg="python3 -t $applicfg_path/AppliCfg.py" || AppliCfg="python3 $applicfg_path/AppliCfg.py"
  else
    echo "AppliCfg missing in $applicfg_path"| tee -a "$current_log_file"
    install_cube
  fi
fi


if [[ -z "$xml_config_gen" ]]; then
  xml_config_gen="$provisioning_dir"/SM/Config/SM_Config_General.xml
fi

source "$provisioning_dir/SM/Helper/update_nsapp_setup.sh" $compil "$exedir" >>"$current_log_file" 2>&1
ret=$?
if [ $ret -ne 0 ] && [ $ret -ne  2 ]; then
  error_config "nsapp"
fi
if [ $ret -eq 2 ]; then
  echo "xml have been copied from $provisioning_dir/SM/Images"
fi
success_config

