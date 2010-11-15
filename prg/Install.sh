#!/bin/sh -u
WRKDIR=`pwd`
echo $WRKDIR
echo $CLIPROOT
NODBU=""
export DESTDIR=""

[ -n "$MAKE" ] || MAKE=make
export MAKE
set -e
if [ "$NODBU" = yes ]
then
	nodbu=yes
else
	nodbu=
fi
for arg in $*
do
	case "$arg" in
	-nodbu)
		nodbu=yes
		;;
	esac
done
DirName="
bdbfs
cobra_clnt
cobra_serv
codb
dbc
debug
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
	[ -d $dName ] && cd $dName && $MAKE install && cd ..
done
if [ "$CLIPROOT" != "$WRKDIR" ]
then
	cp -R locale.po $DESTDIR$CLIPROOT
fi
$CLIPROOT/bin/clip_msgmerge$SCRIPTSUFF
$CLIPROOT/bin/clip_msgfmt$SCRIPTSUFF
exit 0

if [ -d utils ]
then
	cd utils; $MAKE install; cd ..
fi
if [ -d kamache ]
then
	cd kamache; $MAKE install; cd ..
fi

if [ -d doc_utils ]
then
	cd doc_utils; $MAKE install; cd ..
fi

if [ -d cobra_serv ]
then
	cd cobra_serv; $MAKE install; cd ..
fi

if [ -d cobra_clnt ]
then
	cd cobra_clnt; $MAKE install; cd ..
fi

if [ -d debug ]
then
	cd debug; $MAKE install; cd ..
fi

if [ -d pp ]
then
	cd pp; $MAKE install; cd ..
fi

rm -f $CLIPROOT/bin/dbu*

if [ "$nodbu" != 'yes' ]
then

	if [ -d ca_dbu ]
	then
		cd ca_dbu
		$MAKE install
		cd ..
	fi
	if [ -d ca_dbu53 ]
	then
		cd ca_dbu53
		$MAKE install
		cd ..
	fi

fi

if [ -d hiper ]
then
	cd hiper
	$MAKE install
	cd ..
fi

if [ -d bdbfs ]
then
	cd bdbfs
	$MAKE install
	cd ..
fi

if [ -d dbc ]
then
	cd dbc
	$MAKE install
	cd ..
fi
if [ -d udb ]
then
	cd udb
	$MAKE install
	cd ..
fi
if [ -d udbx ]
then
	cd udbx
	$MAKE install
	cd ..
fi
if [ -d ocmng ]
then
	cd ocmng
	$MAKE install
	cd ..
fi
if [ -d codb ]
then
	cd codb
	$MAKE install
	cd ..
fi
#
if [ "$CLIPROOT" != "$WRKDIR" ]
then
	cp -R locale.po $DESTDIR$CLIPROOT
fi
$CLIPROOT/bin/clip_msgmerge$SCRIPTSUFF
$CLIPROOT/bin/clip_msgfmt$SCRIPTSUFF

