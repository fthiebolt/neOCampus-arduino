#!/bin/bash
#
# neOCampus operation
#
# Deploying specific settings for board
#
# F.Thiebolt    aug.22  update for 2.0.4 sdk with esp32 target only
#                       i.e does not apply to esp32 derivatives like esp32c3
# F.Thiebolt    aug.21  initial release
#

#set -x

# checks
[ $# -lt 1 ] && { echo -e "\n###ERROR missing base dir parameter !" >&2; exit 1; }
[ -d $1 ] || { echo -e "\n###ERROR non existing directory '$1'!" >&2; exit 1; }

echo -e "Copying additional board files ..."

# copying files
cp -af opt.h $1/esp32/include/lwip/lwip/src/include/lwip/
[ $? -ne 0 ] && { echo -e "\n###ERROR copying some files to '$1/esp32/include/lwip/lwip/src/include/lwip/'" >&2; exit 1; }
chmod a+r $1/esp32/include/lwip/lwip/src/include/lwip/opt.h

cp -af *.a $1/esp32/lib/
[ $? -ne 0 ] && { echo -e "\n###ERROR copying some files to '$1/esp32/lib/'" >&2; exit 1; }
chmod a+r $1/esp32/lib/*.a

#set +x

