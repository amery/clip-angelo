#!/bin/bash -u
	#
	# erases passed directories
	#

	if [ -z "$*" ] ; then
		echo "usage is : $0 DIR " >&0

		exit 1
	fi
	CurDir="$*"
	let jj=-1
	for ii in $CurDir ; do
		let ++jj
	done
	if [ $jj -gt 0 ] ; then
		echo dir name error echo $* >&0
		echo "usage is : $0 DIR " >&0
		exit 1
	fi
	cd "$*"

	#[ -f bin/functions ] && source bin/functions
	export >>/dev/null

function CleanCVS ()
	{
		local DirList
		local fName
		local CurDir="$PWD"
		local gg
		local CurDone
		echo scanning $PWD 														>&0
		if [ -d CVS ] ; then
			rm -Rfv CVS
		fi
		if [ -d debian ] ; then
			rm -Rfv debian
		fi
		DirList=$(ls -A)
		for fName in $DirList ; do
			if [ -L $fName ] ; then
				echo $fName is a link
			elif [ -b $fName ] ; then
				echo $fName is a block
			elif [ -c $fName ] ; then
				echo $fName is a character
			elif [ -h $fName ] ; then
				echo $fName is a link
			elif [ -p $fName ] ; then
				echo $fName is a pipe
			elif [ -t $fName ] ; then
				echo $fName is a terminal
			elif [ -S $fName ] ; then
				echo $fName is a socket
			elif [ -d $fName ] ; then
				cd "$CurDir/$fName"
				CleanCVS $CurDir/$fName
				cd "$CurDir"
			fi
		done
	}
	CleanCVS
