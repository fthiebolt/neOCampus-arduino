#!/bin/bash
#
# Deployment script for neOCampus / neOSensor ESP32 based boards
#
# F.Thiebol     aug.20  initial release
#

#
# Global defs
ESP32_REV=${ESP32_REV:-"1.0.4"}
ESP32_DIR=${ESP32_DIR:-"~/.arduino15/packages/esp32/hardware/esp32/${ESP32_REV}"}
eval ESP32_DIR=${ESP32_DIR}
ESP32_SRCDIR="arduinoIDE_esp32_boards"

ESP8266_REV=${ESP32_REV:-"2.7.4"}
ESP8266_DIR=${ESP32_DIR:-"~/.arduino15/packages/esp8266/hardware/esp8266/${ESP8266_REV}"}
eval ESP8266_DIR=${ESP32_DIR}
ESP8266_SRCDIR="arduinoIDE_esp8266_boards"

#
# function install
# $1: ARCH (e.g ESP8266 or ESP32)
# $2: SDK_REV (e.g "2.7.4" for esp8266 "1.0.4" for esp32)
function boards_install() {
    [ $# -ne 2 ] && { return 1; }
    
}

TO BE CONTINUED

# usage
echo -e "\n############################################################"
echo -e   "#                                                          #"
echo -e   "#        neOCampus ESP8266/ESP32 boards installer          #"
echo -e   "#                                                          #"
echo -e   "# -------------------------------------------------------- #"

# check if target directory exists
[ -d ${ESP32_DIR} ] || { echo -e "###ERROR: unable to find dir '${ESP32_DIR}' ... aborting" >&2; exit 1; }

echo -e   "# Detected esp32 install dir:                              #"
printf    "%-80s\n" "${ESP32_DIR}"
echo -e   "# -------------------------------------------------------- #"
sleep 1

# parse current variants directory
echo -e   "#         install boards                                   #"
echo -e   "#                                                          #"
_cpt=0
for variant_dir in $(/bin/ls -d */); do
    # is it really a variant dir ??
    _vdir=${variant_dir%%/}
    [ -f ${_vdir}/variant.h ] || { continue; }
    echo -ne "${_vdir} --> install esp32 board [y/N]? : "
    read -e -n 1 answer
    [ "X${answer,,}" != "Xy" ] && { echo -e "cancelled!"; continue; }
    # copy variant dir
    _destdir=${ESP32_DIR}/variants/
    rsync -av --delete --dry-run ${_vdir} ${_destdir} >& /dev/null
    [ $? -ne 0 ] && { echo -e "\n#WARNING: unable to rsync ${_vdir} in ${_destdir} ... continuing!" >&2; sleep 2; continue; }
    rsync -av --delete ${_vdir} ${_destdir}
    [ $? -ne 0 ] && { echo -e "\n### ERROR while rsync of ${_vdir} in ${_destdir} ... aborting!" >&2; exit 1; }
    echo -e "\tsuccessfully installed board '${_vdir}' :)"
    (( _cpt++ ))
done
echo -e   "#                                                          #"
echo -e   "# -------------------------------------------------------- #"
#[aug.20] some boards does not have variant defs!
#[ "X${_cpt}" == "X0" ] && { exit 0; }

# copy local defs files
echo -e   "#         copy local files                                 #"
echo -e   "#                                                          #"
for local_file in $(/bin/ls *local.txt); do
    if [ -f ${ESP32_DIR}/${local_file} ]; then
        echo -ne "overwrite existing ${ESP32_DIR}/${local_file} file [y/N]? :"
        read -e -n 1 answer
        [ "X${answer,,}" != "Xy" ] && { echo -e "cancelled!"; continue; }
    fi
    cp -af ${local_file} ${ESP32_DIR}/ >& /dev/null
    [ $? -ne 0 ] && { echo -e "\n### ERROR while copying '${local_file}' file to '${ESP32_DIR}' directory ... aborting!" >&2; exit 1; }
    echo -e "\tsuccessfully installed local file '${local_file}'"
done
echo -e   "#                                                          #"
echo -e   "# -------------------------------------------------------- #"

# finialize
echo -e "\nYou may restart your arduino toolchain to have your variants taken into account"
echo -e "\thave a nice day :)"

# The end - Jim Morrison / 1943 - 1971

