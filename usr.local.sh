#!/bin/bash -u
init/compile.sh "usr.local"

[ -f ./usr.local ] && rm -fv ./usr.local
