#!/bin/bash -u
init/compile.sh "home"

[ -f ./home ] && rm -fv ./home
