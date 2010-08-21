static function te_saveBlock( fileName, createbak )
local nfile, nbakfile, i, n, str, scr, ps, pl, ts, hch, ch
  save screen to scr
  filename := alltrim(filename)
  createbak := iif(createbak==NIL, .f., createbak)
  @ maxrow(), 0 say padr([Save block to file...], maxcol()) color "0/7"
  inkey()
  /* create new or truncate to 0 exists file */
  if !empty(createbak) .and. createbak
	nbakfile := frename(filename, filename+".bak")
	if nbakfile < 0
		alert([Can't create ;]+filename+[.bak file], "OK")
	endif
  endif
  nfile:=fcreate(filename, 1)
  if nfile<0
     restore screen from scr
     return .f.
  endif
  hch := ::__hostcharset
  ch := ::charset
  if ::strblock
	ts := ::tabSize
	i := min(::koordblock[1], ::koordblock[3])
	n := max(::koordblock[1], ::koordblock[3]) - i + 1
	if ::charset ==NIL
		if ::tabPack
			aeval(::edbuffer, {|item| ;
				str:=tabpack(padr(item, len(item)-len(ltrim(item))), ts)+ltrim(item)+"&\n", ;
				fwrite(nfile, str, len(str))}, i, n)
		else
			aeval(::edbuffer, {|item| ;
				str:=item+"&\n",fwrite(nfile, str, len(str))}, i, n)
		endif
	else
		if ::tabPack
			aeval(::edbuffer, {|item| ;
				str:=tabpack(padr(item, len(item)-len(ltrim(item))), ts)+ltrim(item), ;
				str:=translate_charset(hch, ch, str)+"&\n", ;
				fwrite(nfile, str, len(str))}, i, n)
		else
			aeval(::edbuffer, {|item| ;
				str:=translate_charset(hch, ch, item)+"&\n",;
				fwrite(nfile, str, len(str))}, i, n)
		endif
	endif
  else
	ps := min(::koordblock[2], ::koordblock[4])
	pl := max(::koordblock[2], ::koordblock[4]) - ps +1
	i := min(::koordblock[1], ::koordblock[3])
	n := max(::koordblock[1], ::koordblock[3]) - i + 1
	if ::charset == NIL
		aeval(::edbuffer, {|item| ;
			str:=substr(item, ps, pl)+"&\n", ;
			fwrite(nfile, str, len(str))}, i, n)
	else
		aeval(::edbuffer, {|item| ;
			str:=translate_charset(hch, ch, substr(item, ps, pl))+"&\n", ;
			fwrite(nfile, str, len(str))}, i, n)
	endif
  endif
  fclose(nfile)
  ::updated:=.f.
  restore screen from scr
RETURN .t.
