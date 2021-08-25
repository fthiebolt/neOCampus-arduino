#!/bin/bash
#
# neOCampus operation
#
# Deploying specific settings for board
#
# F.Thiebolt    aug.21  initial release
#

#set -x

# checks
[ $# -lt 1 ] && { echo -e "\n###ERROR missing base dir parameter !" >&2; exit 1; }
[ -d $1 ] || { echo -e "\n###ERROR non existing directory '$1'!" >&2; exit 1; }

echo -e "Copying additional board files ..."

# copying files
cp -af opt.h $1/include/lwip/lwip/
[ $? -ne 0 ] && { echo -e "\n###ERROR copying some files to '$1/include/lwip/lwip/'" >&2; exit 1; }
chmod a+r $1/include/lwip/lwip/opt.h

cp -af *.a $1/lib/
[ $? -ne 0 ] && { echo -e "\n###ERROR copying some files to '$1/lib/'" >&2; exit 1; }
chmod a+r $1/lib/*.a

#set +x

