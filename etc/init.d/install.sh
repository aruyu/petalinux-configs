#!/bin/bash
#==
#   NOTE      - install.sh
#   Author    - Chae Lee-Jin
#
#   Created   - 2024.02.14
#   Github    - https://github.com/aruyu
#   Contact   - vine9151@gmail.com
#/



T_CO_RED='\e[1;31m'
T_CO_YELLOW='\e[1;33m'
T_CO_GREEN='\e[1;32m'
T_CO_BLUE='\e[1;34m'
T_CO_GRAY='\e[1;30m'
T_CO_NC='\e[0m'

CURRENT_PROGRESS=0

function script_print()
{
  echo -ne "${T_CO_BLUE}[SCRIPT]${T_CO_NC}$1"
}

function script_notify_print()
{
  echo -ne "${T_CO_BLUE}[SCRIPT]${T_CO_NC}${T_CO_GREEN}-Notify- $1${T_CO_NC}"
}

function script_error_print()
{
  echo -ne "${T_CO_BLUE}[SCRIPT]${T_CO_NC}${T_CO_RED}-Error- $1${T_CO_NC}"
}

function script_println()
{
  echo -ne "${T_CO_BLUE}[SCRIPT]${T_CO_NC}$1\n"
}

function script_notify_println()
{
  echo -ne "${T_CO_BLUE}[SCRIPT]${T_CO_NC}${T_CO_GREEN}-Notify- $1${T_CO_NC}\n"
}

function script_error_println()
{
  echo -ne "${T_CO_BLUE}[SCRIPT]${T_CO_NC}${T_CO_RED}-Error- $1${T_CO_NC}\n"
}

function error_exit()
{
  script_error_println "$1\n"
  exit 1
}




#==
#   Starting codes in blew
#/

if [[ $EUID -eq 0 ]]; then
  error_exit "This script must be run as USER!"
fi


if [[ $PWD != *"init.d" ]]; then
  error_exit "This script must be run in '/etc/init.d/'!"
fi


script_notify_println "Start changing mode."

chmod a=rx startupd syncp || error_exit "Change mode failed."
chmod u=rwx startupd syncp || error_exit "Change mode failed."

script_notify_println "Start manually chkconfig --level 0 1 2 3 4 5 6."

cd ../rc0.d/
ln -s ../init.d/startupd ../rc0.d/K20startupd || error_exit "Create link in rc0.d failed."
ln -s ../init.d/startupd ../rc1.d/K20startupd || error_exit "Create link in rc1.d failed."
ln -s ../init.d/startupd ../rc2.d/S20startupd || error_exit "Create link in rc2.d failed."
ln -s ../init.d/startupd ../rc3.d/S20startupd || error_exit "Create link in rc3.d failed."
ln -s ../init.d/startupd ../rc4.d/S20startupd || error_exit "Create link in rc4.d failed."
ln -s ../init.d/startupd ../rc5.d/S20startupd || error_exit "Create link in rc5.d failed."
ln -s ../init.d/startupd ../rc6.d/K20startupd || error_exit "Create link in rc6.d failed."

script_notify_println "Install additional apps."

mv ../init.d/syncp ../../bin/

script_notify_println "All successfully done."


while true; do
  echo
  read -p "You want to delete 'install.sh'? (y/N) " SELECTION
  case ${SELECTION} in
    [Yy] )
      rm ../init.d/install.sh
      script_notify_println "Deleted 'install.sh'."
      break
      ;;

    [N] )
      script_notify_println "Didn't delete 'install.sh'."
      break
      ;;

    * )
      script_error_println "Wrong answer. (y/N)"
  esac
done
