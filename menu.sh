#!/bin/bash -u
[ -f ../vTempVersion.txt ] && cp -fpuv ../vTempVersion.txt ./
vTempVersion="$(cat ./vTempVersion.txt)"
export vTempVersion

echo vTempVersion 1: $vTempVersion $PWD
exec init/menu.Make.sh
