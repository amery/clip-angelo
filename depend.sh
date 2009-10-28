#!/bin/bash -u
echo Started at : 	>config/start.depend
date +%T 				>>config/start.depend
date +%T
date +%T >../depend
init/compile.sh "depend"
[ -f ./home ] && rm -fv ./home
[ -f ./depend ] && rm -fv ./depend
echo Ended at : 		>>config/start.depend
date +%T 				>>config/start.depend
echo On system :		>>config/start.depend
uname -o 				>>config/start.depend
uname -s 				>>config/start.depend
uname -r 				>>config/start.depend
uname -v 				>>config/start.depend
uname -p 				>>config/start.depend
date +%T
