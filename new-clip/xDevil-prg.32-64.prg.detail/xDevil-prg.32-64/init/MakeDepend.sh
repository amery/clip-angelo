#!/bin/sh
#
source ./configure.ini
iName=""
fName=""
i_files=""
[ -L $PWD ] && exit 0
#SRC_C_files=""
for fName in $SRC_C_files ; do
	echo "Checking dependencies for $PWD/$fName" >&0
	echo "Checking dependencies for $PWD/$fName" >&1
	echo "Checking dependencies for $PWD/$fName" >&2
	if [ -f ./$fName ] ; then
		echo "Line : " $C_FLAGS $fName >&1
		echo "Line : " $C_FLAGS $fName >&2
#		xDevil_mk_depend$EXESUFF $C_FLAGS $fName
		i_files="$(xDevil_mk_depend$EXESUFF $C_FLAGS $fName)"
##		i_files="$(gcc -M $C_FLAGS $fName)"
#		echo $iName $fName >>$xDevil_H_Dir/temp.c
		for iName in $i_files ; do
			if [[ -f $iName ]] ; then
				if [[ $iName -nt $fName ]] ; then
					touch $fName
					echo u $iName $fName >>$xDevil_H_Dir/temp.txt
				fi
			fi
		done
	fi
done
for fName in $SRC_PRG_files ; do
	echo "Checking dependencies for $PWD/$fName" >&0
	echo "Checking dependencies for $PWD/$fName" >&1
	echo "Checking dependencies for $PWD/$fName" >&2
	if [ -f ./$fName ] ; then
		echo "Line : " $xDevilFLAGS $fName >&1
		echo "Line : " $xDevilFLAGS $fName >&2
#		xDevil_mk_depend$EXESUFF $xDevilFLAGS $fName
		i_files="$(xDevil_mk_depend$EXESUFF $xDevilFLAGS $fName)"
		for iName in $i_files ; do
#			echo $iName $fName >>$xDevil_H_Dir/temp.prg
			if [[ -f $iName ]] ; then
				if [[ $iName -nt $fName ]] ; then
					touch $fName
					echo u $iName $fName >>$xDevil_H_Dir/temp.txt
				fi
			fi
		done
	fi
done

