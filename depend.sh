#!/bin/bash -ux
init/compile.sh "depend"
date +%T
date +%T >../depend
[ -f ./home ] && rm -fv ./home
[ -f ./depend ] && rm -fv ./depend
date +%T >>../depend
