#!/bin/bash -ux
init/compile.sh "opt"

[ -f ./opt ] && rm -fv ./opt
