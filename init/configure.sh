#!/bin/bash -u
echo "Configuring $PWD" >&0
source ../configure.ini
cp --remove-destination -fu$V ../Makefile.ini ../configure.ini ./
if ! [ -f ./configure ] ; then
	if [ -f ./configure.in ] ; then
		echo "#!/bin/bash " 										>./configure
		cat ./configure.ini 										>>./configure
		echo "source $Clip_M_Dir/init/functions.f" 		>>./configure
		cat ./configure.in  										>>./configure
		chmod +x ./configure
	fi
fi
[ -f configure ] && ./configure 0
cat ./Makefile.ini 								>Makefile
[ -f Makefile.inc ] && cat Makefile.inc 	>>Makefile
if [ -f Makefile.tmp ] ; then
	cat Makefile.tmp 								>>Makefile
else
	cat ./Makefile.in 							>>Makefile
fi