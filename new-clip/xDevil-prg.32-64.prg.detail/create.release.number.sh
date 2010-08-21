#!/bin/sh -ux
YearDay=$(date +%Y.%j)
seq_no=$(cat seq_no.txt)
[ -d xDevil-prg.32-64 ] || echo "xDevil-prg.32-64 DOES NOT EXIST"
[ -d xDevil-prg.32-64 ] || exit 1
[ -d xDevil-prg.32-64 ] && echo Release will be : xDevil-prg.32-64-$YearDay-$seq_no
[ -d xDevil-prg.32-64 ] && read -n 1 -p "Press ctrl+C to break"
[ -d xDevil-prg.32-64 ] && cd xDevil-prg.32-64
echo $(date +%Y-%m-%d) 	>release.date
echo $YearDay 				>release.sequence
echo $seq_no 				>release.version
cd ..
[ -d xDevil-prg.32-64 ] && echo "Create xDevil-prg.32-64-$YearDay-$seq_no.tgz"
[ -d xDevil-prg.32-64 ] && read -n 1 -p "Press ctrl+C to break"
rm -fv xDevil-prg.32-64-$YearDay-$seq_no.tgz
tar -cvzf xDevil-prg.32-64-$YearDay-$seq_no.tgz xDevil-prg*
