#!/bin/bash
#ADDMAKECMDGOALS="$*"
#while [ $# -le 1 ] ; do
#	shift
#done
#if [ -n "$ADDMAKECMDGOALS" ] ; then
#	error $PWD
#fi
Listname=""
DirName=""
Listname="$(ls)"
[ -f ./configure.in ] || exit 0
#[ -f $CONFIGURE ] && ! [ -f ./configure.in ] && mv -f$V $CONFIGURE ./configure.in
[ -f ./configure.in ] && [ ./configure.in -nt $CONFIGURE ] &&  rm -f$V $CONFIGURE
if ! [ -f $CONFIGURE_SH ] ; then
	echo "Configuration for $PWD"
#	[ -f ./configure.in ] && cp -f$V ./configure.in ./configure.sav
	echo "#!/bin/bash " 															>$CONFIGURE
	echo "#" 																		>>$CONFIGURE
	echo "# auto configure generated by main configure" 				>>$CONFIGURE
	echo "[ -f $xDevil_F_CFG ]  && source $xDevil_F_CFG" 				>>$CONFIGURE
	echo "[ -f ../configure.ini ] && source ../configure.ini" 		>>$CONFIGURE
	echo "source $xDevil_F_F" 													>>$CONFIGURE
	echo "[ -f ./configure.ini ] && source ./configure.ini" 			>>$CONFIGURE
	echo "Config_Init" 															>>$CONFIGURE
	cat ./configure.in 															>>$CONFIGURE
#	touch dummy.c
	chmod +x $CONFIGURE
	sleep .1
	$CONFIGURE
	if [ $? != 0 ] ; then
		echo "Configuration for $PWD" $0 $LINENO
		error $PWD
		exit 1
	fi
	echo "Configuration for $PWD success"
fi

for Dirname in $Listname ; do
	if [ -L $Dirname ] ; then
		echo "">/dev/null
	elif [ -d $Dirname ] ; then
		cd $Dirname
		$CONFIGURE_DIR
		cd ..
	fi
done
