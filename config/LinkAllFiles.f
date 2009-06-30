#
# called from configure files
#
		fName=""
		H_f="$h_files"
		CH_f="$ch_files"
		C_f="$c_files"
		P_f="$prg_files"
		O_f="$OBJ_files"
		echo $PWD progname : $ProgName
		echo linking : $PWD to $Hash_Dir
		OBJ_files=""
		c_files=""
		for fName in $HASHSRC ; do
			if ! [ -f ./$fName ] ; then
				error $PWD $fName does not exist
			fi
  			ln -sf$V "$PWD/$fName" "$Clip_M_Dir/hasher/hashed/$ProgName-$fName"
		done
		for fName in $C_f ; do
			if [ -f ./$fName.c ] ; then
				c_files="$c_files $fName.c"
				OBJ_files="$OBJ_files $fName.o"
			fi
		done
		ch_files=""
		for fName in $CH_f ; do
			if [ -f ./$fName ] ; then
				ch_files="$ch_files $fName"
				cp -f$V ./$fName $Clip_I_Dir/
			fi
		done
		h_files=""
		for fName in $H_f ; do
			if [ -f ./$fName ] ; then
				h_files="$h_files $fName"
				cp -f$V ./$fName $Clip_I_Dir/
			fi
		done
		prg_files=""
		for fName in $P_f ; do
			if [ -f ./$fName.prg ] ; then
				prg_files="$prg_files $fName.prg"
				OBJ_files="$OBJ_files $fName.o"
			fi
		done
		if [ -L include ] || [ -d include ] ; then
			echo >/dev/null
		else
			ln -sf$V $Clip_I_Dir ./include
		fi
		if [ -L charsets ] || [ -d charsets ] ; then
			echo >/dev/null
		else
			ln -sf$V $CLIPROOT/charsets ./
		fi
		if [ -L locale.po ] || [ -d locale.po ] ; then
			echo >/dev/null
		else
			ln -sf$V $CLIPROOT/locale.po ./
		fi
		[ -f $Clip_I_Dir/include ] || rm -f$V $Clip_I_Dir/include
		export prg_files="$prg_files"
		export ch_files="$ch_files"
		export c_files="$c_files"
		export h_files="$h_files"
		export OBJ_files="$OBJ_files"
		echo "export prg_files=\"$prg_files\""	 											>>configure.ini
		echo "export ch_files=\"$ch_files\""	 											>>configure.ini
		echo "export c_files=\"$c_files\""	 												>>configure.ini
		echo "export h_files=\"$h_files\""	 												>>configure.ini
		echo "export OBJ_files=\"$OBJ_files\""	 											>>configure.ini

