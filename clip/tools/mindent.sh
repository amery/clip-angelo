#!/bin/bash -u

SIMPLE_BACKUP_SUFFIX=.BAK
export SIMPLE_BACKUP_SUFFIX
indent -l128 -lc80 -bad -bap -sob -nfc1 -d1 -ncdb -cd33 -bli0 -npcs \
	 -di1 -nbc -psl -i8 -ip8 -lp -ci0 -nsc -nce $*
