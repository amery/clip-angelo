#!/bin/sh -u
WRKDIR=`pwd`
echo $WRKDIR
echo $CLIPROOT
NODBU=""
[ -n "$MAKE" ] || MAKE=make
export MAKE
set -e
if [ "$NODBU" = yes ] ; then
	nodbu=yes
else
	nodbu=
fi
for arg in $* ; do
	case "$arg" in
	-nodbu)
		nodbu=yes
		;;
	esac
done
DirName="
debug
bdbfs
cobra_clnt
cobra_serv
codb
dbc
doc_utils
hiper
kamache
ocmng
pp
udb
udbx
utils
"
for dName in $DirName ; do
	[ -d $dName ] && cd $dName && $MAKE && cd ..
done
exit 0
if [ -d utils ] ; then
	cd utils; $MAKE; cd ..
fi
if [ -d debug ] ; then
	cd debug; $MAKE; cd ..
fi
if [ -d kamache ] ; then
	cd kamache; $MAKE; cd ..
fi
if [ -d pp ] ; then
	cd pp; $MAKE; cd ..
fi
rm -f $CLIPROOT/bin/dbu*
if [ -d hiper ] ; then
	cd hiper
	$MAKE
	cd ..
fi
if [ -d doc_utils ] ; then
	cd doc_utils
	$MAKE
	cd ..
fi
if [ -d cobra_serv ] ; then
	cd cobra_serv
	$MAKE
	cd ..
fi
if [ -d cobra_clnt ] ; then
	cd cobra_clnt
	$MAKE
	cd ..
fi
if [ -d bdbfs ] ; then
	cd bdbfs
	$MAKE
	cd ..
fi
if [ -d dbc ] ; then
	cd dbc
	$MAKE
	cd ..
fi
if [ -d udb ] ; then
	cd udb
	$MAKE
	cd ..
fi
if [ -d udbx ] ; then
	cd udbx
	$MAKE
	cd ..
fi
if [ -d ocmng ] ; then
	cd ocmng
	$MAKE
	cd ..
fi
if [ -d codb ] ; then
	cd codb
	$MAKE
	cd ..
fi

