#!/bin/bash -u
date +%T >>depend
cd xDevil-prg.32-64
./depend.sh
date +%T >>depend