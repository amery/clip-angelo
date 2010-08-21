#!/bin/bash -u
if ! [ -d xDevil-prg.32-64 ] ; then
	mkdir xDevil-prg.32-64
	cd xDevil-prg.32-64
	[ -d $HOME/xDevil/xDevil-prg.32-64.prg.detail/xDevil-prg.32-64 ] \
		&& cp -Rfpuv $HOME/xDevil/xDevil-prg.32-64.prg.detail/xDevil-prg.32-64/* ./
fi
cd xDevil-prg.32-64
./menu.sh
