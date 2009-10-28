#!/bin/bash -u
export WaitTime=1
init/compile.sh "local"
[ -f ./local ] && rm -fv ./local
