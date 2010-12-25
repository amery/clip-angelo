/*
    Copyright (C) 2003  ITK
    Author   : Alexey M. Tkachenko (alexey@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

#include <ci_clip-gtk2.ch>
#include "ci_abx.ch"

function main()
	local abx

	abx := abx_new()

	abx:init()
	abx:run()
	abx:done()
return 0