#!/bin/bash
#
# Deployment script for neOCampus / neOSensor boards
#
# F.Thiebolt    jul.23  upgrades esp* SDKs
# F.Thiebolt    jan.23  upgrades esp* SDKs
# F.Thiebolt    aug.22  upgrades esp* SDKs
# F.Thiebolt    feb.21  added CubeCell support
# F.Thiebolt    sep.20  force SDK 2.7.1 for esp8266 (2.7.4 exhibits WiFi failures
#                       in higly degraded WiFi environment).
# F.Thiebolt    aug.20  initial release
#

#
# Global defs
ESP32_REV=${ESP32_REV:-"2.0.14"}
ESP8266_REV=${ESP8266_REV:-"3.1.2"}
CUBECELL_REV=${CUBECELL_REV:-"1.3.0"}


#
# function install
# $1: ARCH (e.g ESP8266, ESP32 or CubeCell)
# $2: SDK_REV (e.g "2.7.4" for esp8266 "1.0.4" for esp32)
function boards_install() {
    [ $# -ne 2 ] && { return 1; }
    ARCH=$1
    SDK_REV=$2

    SDK_DIR="~/.arduino15/packages/${ARCH}/hardware/${ARCH}/${SDK_REV}"
    eval SDK_DIR=${SDK_DIR}
    [ -d ${SDK_DIR} ] || { echo -e "unable to find ${ARCH} SDK dir '${SDK_DIR}' ... wrong revision ??" >&2; return 1; }

    echo -e "\n# -------------------------------------------------------- #"
    echo -e "#\tTARGET:\t${ARCH^^}"
    echo -e "#\tDetected ${ARCH} SDK dir:"
    printf  "#\t%-80s\n" "${SDK_DIR}"
    sleep 1

    SRC_DIR="arduinoIDE_${ARCH,,}_boards"
    [ -d ${SRC_DIR} ] || { echo -e "unable to find dir '${SRC_DIR}' ... aborting" >&2; echo -e "# -------------------------------------------------------- #"; return 1; }

    #
    # parse current variants directory
    echo -e "#         install boards                                   #"
    echo -e "#                                                          #"
    _cpt=0
    for variant_dir in $(/bin/ls -d ${SRC_DIR}/*); do
        # [sep.20] NOT TESTED !!
        # is it really a variant dir ??
        _variant_dir=${variant_dir#*/}
        _vdir=${_variant_dir%%/}
        [ -f ${_vdir}/variant.h ] || { continue; }
        echo -ne "${_vdir} --> install ${ARCH,,} board(s) [y/N]? : "
        read -e -n 1 answer
        [ "X${answer,,}" != "Xy" ] && { echo -e "cancelled!"; continue; }
        # copy variant dir
        _destdir=${SDK_DIR}/variants/
        rsync -av --delete --dry-run ${_vdir} ${_destdir} >& /dev/null
        [ $? -ne 0 ] && { echo -e "\n#WARNING: unable to rsync ${_vdir} in ${_destdir} ... continuing!" >&2; sleep 2; continue; }
        rsync -av --delete ${_vdir} ${_destdir}
        [ $? -ne 0 ] && { echo -e "\n### ERROR while rsync of ${_vdir} in ${_destdir} ... aborting!" >&2; exit 1; }
        echo -e "\tsuccessfully installed board '${_vdir}' :)"
        (( _cpt++ ))
    done

    #[aug.20] some boards does not have variant defs!
    #[ "X${_cpt}" == "X0" ] && { exit 0; }

    #
    # copy local defs files
    echo -e "#         copy local files                                 #"
    echo -e "#                                                          #"
    for local_file in $(/bin/ls ${SRC_DIR}/*local.txt); do
        _local_file=${local_file#*/}
        if [ -f ${SDK_DIR}/${_local_file} ]; then
            echo -ne "overwrite existing ${SDK_DIR}/${_local_file} file [y/N]? :"
            read -e -n 1 answer
            [ "X${answer,,}" != "Xy" ] && { echo -e "cancelled!"; continue; }
        fi
        cp -af ${local_file} ${SDK_DIR}/ >& /dev/null
        [ $? -ne 0 ] && { echo -e "\n### ERROR while copying '${local_file}' file to '${SDK_DIR}' directory ... aborting!" >&2; exit 1; }
        echo -e "\tsuccessfully installed local file '${_local_file}'"
    done

    #
    # execute local_deploy script if any
    if [ -x ${SRC_DIR}/local_deploy.sh ]; then
        echo -e "#         execute local_deploy script                      #"
        echo -e "#                                                          #"
        ( cd ${SRC_DIR}; source local_deploy.sh ${SDK_DIR}/tools/sdk; )
    fi

    echo -e "#                                                          #"
    echo -e "# -------------------------------------------------------- #"

    return 0
}


# usage
echo -e "\n############################################################"
echo -e "#                                                          #"
echo -e "#         neOCampus | neOSensor boards installer           #"
echo -e "#                                                          #"
echo -e "# -------------------------------------------------------- #"

# ESP32 boards
boards_install "esp32" ${ESP32_REV}
[ $? -ne 0 ] && { echo -e "\n\tfailed to install additional ESP32 boards ?!?!" >&2; sleep 2; }

# CubeCell boards
boards_install "CubeCell" ${CUBECELL_REV}
[ $? -ne 0 ] && { echo -e "\n\tfailed to install additional CubeCell boards ?!?!" >&2; sleep 2; }

# ESP8266 boards
boards_install "esp8266" ${ESP8266_REV}
[ $? -ne 0 ] && { echo -e "\n\tfailed to install additional ESP8266 boards ?!?!" >&2; sleep 2; }

# finialize
echo -e "\nYou may restart your arduino toolchain to have your variants taken into account"
echo -e "\thave a nice day :)"

# The end - Jim Morrison / 1943 - 1971

