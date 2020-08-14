#!/bin/sh
#
# Copyright 2020 NXP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# parse PACKAGES_PATH and set FIRMWARE_VER based on that

get_git_version()
{
  command -v git>/dev/null 2>&1
  if [ $? -eq 0 ] && [ -n "$1" ]
  then
    head_or_tag=`git -C $1 describe --tag --dirty --broken --always 2>/dev/null`
    printf $head_or_tag
  fi
}

FIRMWARE_VER=
IFS=:

for dir in $PACKAGES_PATH; do
  if [ -n "$FIRMWARE_VER" ]; then
    FIRMWARE_VER="$FIRMWARE_VER;$(basename $dir):$(get_git_version $dir)"
  else
    FIRMWARE_VER=$(basename $dir):$(get_git_version $dir)
  fi
done

unset IFS

echo "FIRMWARE_VER=$FIRMWARE_VER"
export FIRMWARE_VER=$FIRMWARE_VER

echo "build edk2 firmware with -D FIRMWARE_VER=\$FIRMWARE_VER"
