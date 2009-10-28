#!/bin/bash -u
echo "Configuring $PWD because of $*" >&0
echo "Configuring $PWD because of $*" >&1
echo "Configuring $PWD because of $*" >&2
[ -f $PWD/configure.ok ] && rm -f$V $PWD/configure.ok
source $Clip_M_Dir/configure.ini
[ -f ../Makefile.ini ] && cp --remove-destination -fpu$V ../Makefile.ini ../configure.ini ./
[ -f ./Makefile.ini ] || cp --remove-destination -fpu$V $Clip_M_Dir/Makefile.ini $Clip_M_Dir/configure.ini ./
if [ -f ./configure.in ] ; then
	echo "#!/bin/bash -u" 									>./configure
	cat ./configure.ini 										>>./configure
	echo "source $Clip_M_Dir/init/functions.f" 		>>./configure
	cat ./configure.in  										>>./configure
	chmod +x ./configure
fi
[ -f configure ] && ./configure 0
if [ $? != 0 ] ; then
	exit 1
fi
touch $PWD/configure.ok
touch $PWD/configure.ok
touch $PWD/configure.ok
if [[ "$1" = "clean" ]] || [[ "$1" = "distclean" ]] ;  then
	sleep 0
else
	sleep 1
fi
cat ./Makefile.ini 								>Makefile
#[ -f Makefile.inc ] && cat Makefile.inc 	>>Makefile
cat ./Makefile.in 								>>Makefile
echo $PWD/Makefile "is updated" >&0
echo $PWD/Makefile "is updated" >&1
echo $PWD/Makefile "is updated" >&2
if ! [ -f $PWD/configure.ok ] ; then
	echo "error config" >&0
	echo "error config" >&1
	echo "error config" >&2
	exit 1
fi
