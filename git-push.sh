#!/bin/bash
#
# Simple helper script ;)
#

if [ "$#" == "0" ]; then
    msg='update'
else
    msg="$@"
fi

#set -x

# [dec.22] removed StrictHostKey checking
export GIT_SSH_COMMAND="ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no"

git add --all
git commit -a -m "${msg}"
git push

