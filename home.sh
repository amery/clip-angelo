#!/bin/bash -u
export WaitTime=1
init/compile.sh "home"
[ -f ./home ] && rm -fv ./home
