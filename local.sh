#!/bin/bash -u
init/compile.sh "local"
[ -f ./local ] && rm -fv ./local
