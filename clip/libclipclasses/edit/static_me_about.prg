*********************************
static function me_about()
local w, color
	w := wselect()
	color := setcolor("0/7")
	wopen(7, 8, 18, 69)
	wbox()
	@ 1, 2 say padc("TEXT EDITOR", 56)
	@ 2, 2 say padc("Copyright (C) 2001-2004  ITK", 56)
	@ 3, 2 say padc("----------", 56)
	@ 4, 2 say padc("written by CLIP version "+CLIP_VERSION+"-"+alltrim(str(ME_VERSION)), 56)
	@ 6, 2 say padc("Licence : (GPL) http://www.itk.ru/clipper/licence.html", 56)
	@ 7, 2 say padc("Author  : Elena Kornilova <alena@itk.ru>              ", 56)
	@ 8, 2 say padc("< OK >", 56)
	inkey(0)
	wclose()
	wselect(w)
	setcolor(color)
return
