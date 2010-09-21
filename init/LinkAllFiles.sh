#!/bin/bash -u
# called from configure files
#
Hash_Dir=$Clip_M_Dir/hasher/hashed
[ -d $Hash_Dir ] || mkdir -p$V $Hash_Dir
ProgName=$1
h_files=""
ch_files=""
c_files="$(ls -A *.c 2>/dev/null)"
prg_files="$(ls -A *.prg 2>/dev/null)"
i_ch_files=""
i_h_files=""
if [ -L include ] ; then
	echo include is A link
else
	h_files="$(ls -A *.h 2>/dev/null)"
	ch_files="$(ls -A *.ch 2>/dev/null)"
	[ -d include ] && cd include && i_h_files="$(ls -A *.h 2>/dev/null)" && cd ..
	[ -d include ] && cd include && i_ch_files="$(ls -A *.ch 2>/dev/null)" && cd ..
fi
fName=""
H_f="$h_files"
CH_f="$ch_files"
C_f="$c_files"
P_f="$prg_files"
echo $PWD progname : $ProgName
echo linking : $PWD to $Hash_Dir
OBJ_files=""
c_files=""
for fName in $C_f ; do
	if [ -f ./$fName ] ; then
		c_files="$c_files $fName"
		ln -sf$V "$PWD/$fName" "$Hash_Dir/$ProgName-$fName"
	fi
done
for fName in $i_h_files ; do
	if [ -f ./include/$fName ] ; then
		c_files="$c_files $fName"
		ln -sf$V "$PWD/include/$fName" "$Hash_Dir/$ProgName-$fName"
	fi
done
for fName in $i_ch_files ; do
	if [ -f ./include/$fName ] ; then
		c_files="$c_files $fName"
		ln -sf$V "$PWD/include/$fName" "$Hash_Dir/$ProgName-$fName"
	fi
done
ch_files=""
for fName in $CH_f ; do
	if [ -f ./$fName ] ; then
		ch_files="$ch_files $fName"
		ln -sf$V "$PWD/$fName" "$Hash_Dir/$ProgName-$fName"
		cp -f$V ./$fName $Clip_I_Dir/
	fi
done
h_files=""
for fName in $H_f ; do
	if [ -f ./$fName ] ; then
		h_files="$h_files $fName"
		ln -sf$V "$PWD/$fName" "$Hash_Dir/$ProgName-$fName"
		cp -f$V ./$fName $Clip_I_Dir/
	fi
done
prg_files=""
for fName in $P_f ; do
	if [ -f ./$fName ] ; then
		ln -sf$V "$PWD/$fName" "$Hash_Dir/$ProgName-$fName"
		prg_files="$prg_files $fName"
	fi
done

