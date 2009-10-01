#!/bin/bash -u
echo "Configuring $PWD" >&0
source ../configure.ini
cp --remove-destination -fu$V ../Makefile.ini ../configure.ini ./
if [ -f ./configure.in ] ; then
	echo "#!/bin/bash " 										>./configure
	cat ./configure.ini 										>>./configure
	echo "source $Clip_M_Dir/init/functions.f" 		>>./configure
	cat ./configure.in  										>>./configure
	chmod +x ./configure
fi
sleep .1
[ -f configure ] && ./configure 0
sleep .1
cat ./Makefile.ini 								>Makefile
[ -f Makefile.inc ] && cat Makefile.inc 	>>Makefile
if [ -f Makefile.tmp ] ; then
	cat Makefile.tmp 								>>Makefile
else
	cat ./Makefile.in 							>>Makefile
fi
sleep .1
echo $PWD/Makefile "is updated" >&0
echo $PWD/Makefile "is updated" >&1
echo $PWD/Makefile "is updated" >&2
