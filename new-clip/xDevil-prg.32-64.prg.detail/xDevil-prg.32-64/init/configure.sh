#!/bin/bash
echo "Configuring $PWD because of $*" >&0
echo "Configuring $PWD because of $*" >&1
echo "Configuring $PWD because of $*" >&2
for nNames in * ; do
	if [ -d nNames ] ; then
		if [ -f $nNames.h ] ; then
			for SubNames in $nNames/* ; do
				if [ $SubNames -nt $nNames.h ] ; then
					touch $nNames.h
				fi
			done
		fi
		if [ -f $nNames.ch ] ; then
			for SubNames in $nNames/* ; do
				if [ $SubNames -nt $nNames.ch ] ; then
					touch $nNames.ch
				fi
			done
		fi
	fi
done
[ -f $PWD/configure.ok ] && rm -f$V $PWD/configure.ok
source $xDevil_M_Dir/configure.ini
[ -f ../Makefile.ini ] && cp --remove-destination -fpu$V ../Makefile.ini ../configure.ini ./
[ -f ./Makefile.ini ] || cp --remove-destination -fpu$V $xDevil_M_Dir/Makefile.ini $xDevil_M_Dir/configure.ini ./
if [ -f ./configure.in ] ; then
	echo "#!/bin/bash " 										>./configure
	cat ./configure.ini 										>>./configure
	echo "source $xDevil_M_Dir/init/functions.f.sh"	>>./configure
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
if [[ "$1" = "clean" ]] || [[ "$1" = "distclean" ]] || [[ "$1" = "M_config" ]]  ;  then
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
