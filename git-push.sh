#!/bin/bash

if [ "$#" == "0" ]; then
    msg='update'
else
    msg="$@"
fi

set -x
git add --all
git commit -a -m "${msg}"
#git push

