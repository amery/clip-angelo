#!/bin/sh -u
#
source ./configure.ini
iName=""
fName=""
i_files=""
#SRC_C_files=""
for fName in $SRC_C_files ; do
	echo "Checking dependencies for $PWD/$fName" >&0
	echo "Checking dependencies for $PWD/$fName" >&1
	echo "Checking dependencies for $PWD/$fName" >&2
	if [ -f ./$fName ] ; then
		i_files="$(gcc -M $C_FLAGS $fName)"
		echo $iName $fName >>$Clip_H_Dir/temp.c
		for iName in $i_files ; do
			if [[ -f $iName ]] ; then
				if [[ $iName -nt $fName ]] ; then
					touch $fName ./Makefile.in
					echo u $iName $fName >>$Clip_H_Dir/temp.c
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
		clip_mk_depend$EXESUFF $CLIPFLAGS $fName
		i_files="$(clip_mk_depend$EXESUFF $CLIPFLAGS $fName)"
		for iName in $i_files ; do
			echo $iName $fName >>$Clip_H_Dir/temp.prg
			if [[ -f $iName ]] ; then
				if [[ $iName -nt $fName ]] ; then
					touch $fName ./Makefile.in
					echo u $iName $fName >>$Clip_H_Dir/temp.prg
				fi
			fi
		done
	fi
done

