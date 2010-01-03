#!/bin/bash 
#
# erases linked symbol files
#

if [ -z $CLIPROOT ] ; then
	exit 1
fi
cd $Clip_L_Dir
for Cur_File in $(ls) ; do
	echo removing /usr/$LibDir/$Cur_File 	>&0
	rm -iv /usr/$LibDir/$Cur_File || true
done
cd $Clip_B_Dir
for Cur_File in $(ls) ; do
	echo removing /usr/local/bin/$Cur_File 	>&0
	rm -iv /usr/local/bin/$Cur_File || true
	echo removing $HOME/bin/$Cur_File 	>&0
	rm -iv $HOME/bin/$Cur_File || true
done
rm -Riv /usr/bin/clip || true
if [ -n "$*" ] ; then
	while [ -n "$*" ] ; do
		cd $1
		for Cur_File in $(ls) ; do
   		if [ -L /usr/$LibDir/$Cur_File ] ; then
      		echo removing /usr/$LibDir/$Cur_File 	>&0
      		rm -iv /usr/$LibDir/$Cur_File || true
   		fi
		done
		shift
	done
fi
