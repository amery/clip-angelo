#!/bin/bash -u
##
##              checks & creates installation directories
##
set >tmp.set

[ -d $BINDIR ] 					|| mkdir -p$V $BINDIR
[ -d $Clip_B_Dir ] 				|| mkdir -p$V $Clip_B_Dir
[ -d $Clip_I_Dir ] 				|| mkdir -p$V $Clip_I_Dir
[ -d $INSTDIR/charsets ] 		|| mkdir -p$V $INSTDIR/charsets
[ -d $INSTDIR/cliprc ] 			|| mkdir -p$V $INSTDIR/cliprc
[ -d $INSTDIR/cobra ] 			|| mkdir -p$V $INSTDIR/cobra
#[ -d $INSTDIR/codb_ad ] 		|| mkdir -p$V $INSTDIR/codb_ad
[ -d $INSTDIR/doc ] 				|| mkdir -p$V $INSTDIR/doc
[ -d $INSTDIR/etc ] 				|| mkdir -p$V $INSTDIR/etc
[ -d $INSTDIR/kamache ] 		|| mkdir -p$V $INSTDIR/kamache
[ -d $INSTDIR/keymaps ] 		|| mkdir -p$V $INSTDIR/keymaps
[ -d $INSTDIR/lang ] 			|| mkdir -p$V $INSTDIR/lang
[ -d $INSTDIR/lib$C64 ] 		|| mkdir -p$V $INSTDIR/lib$C64
[ -d $INSTDIR/lib ] 				|| ln -sf$V $INSTDIR/lib$C64 $INSTDIR/lib
[ -d $INSTDIR/locale.mo ] 		|| mkdir -p$V $INSTDIR/locale.mo
[ -d $INSTDIR/locale.po ] 		|| mkdir -p$V $INSTDIR/locale.po
[ -d $INSTDIR/locale.pot ] 	|| mkdir -p$V $INSTDIR/locale.pot
[ -d $INSTDIR/term ] 			|| mkdir -p$V $INSTDIR/term
[ -d $Clip_M_Dir/temp ] 		|| mkdir -p$V $Clip_M_Dir/temp
