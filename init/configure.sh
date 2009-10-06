#!/bin/bash -u
echo "Configuring $PWD" >&0
[ -f $PWD/configure.ok ] && rm -f$V $PWD/configure.ok
source $Clip_M_Dir/configure.ini
cp --remove-destination -fu$V ../Makefile.ini ../configure.ini ./
if [ -f ./configure.in ] ; then
	echo "#!/bin/bash -u" 									>./configure
	cat ./configure.ini 										>>./configure
	echo "source $Clip_M_Dir/init/functions.f" 		>>./configure
	cat ./configure.in  										>>./configure
	chmod +x ./configure
fi
sleep .1
[ -f configure ] && ./configure 0
if [ $? != 0 ] ; then
	break
fi
touch $PWD/configure.ok
touch $PWD/configure.ok
touch $PWD/configure.ok
sleep .1
cat ./Makefile.ini 								>Makefile
#[ -f Makefile.inc ] && cat Makefile.inc 	>>Makefile
cat ./Makefile.in 								>>Makefile
sleep .1
echo $PWD/Makefile "is updated" >&0
echo $PWD/Makefile "is updated" >&1
echo $PWD/Makefile "is updated" >&2
if ! [ -f $PWD/configure.ok ] ; then
	echo "error config" >&0
	echo "error config" >&1
	echo "error config" >&2
	break
fi
