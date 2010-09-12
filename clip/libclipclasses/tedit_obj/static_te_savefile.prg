static function te_saveFile( filename, createbak )
local nfile, nbakfile, i, n, str, scr, newf, error
  //save screen to scr
  scr := saveScreen(0,0,maxRow(),maxCol(),.F.)
  if empty(filename)
	filename := ::FileName
  endif
  filename := alltrim(filename)
  createbak := iif(createbak==NIL, .f., createbak)
  @ maxrow(), 0 say padr([Save file...], maxcol()) color "0/7"
  inkey()
  newf := !(file(filename))
  /* create new or truncate to 0 exists file */

  BEGIN SEQUENCE

  ERRORBLOCK({|e| alert(e:description+";"+[file ;]+e:args[2])})

  if !empty(createbak) .and. createbak .and. !newf
	nbakfile := __copyFile(filename, filename+".bak")
	if nbakfile == 0
		alert([Can't create ;]+filename+[.bak file], "OK")
	endif
  endif
  RECOVER USING error
	outlog(__FILE__, __LINE__, error:description, error:operation, error:args)
  ENDSEQUENCE
  if newf
	nfile:=fcreate(filename, FO_READWRITE)
  else
	nfile:=fopen(filename, FO_READWRITE+FO_TRUNC)
  endif
  if nfile<0
     //restore screen from scr
     restScreen(0, 0, maxRow(), maxCol(), scr)
     return .f.
  endif
  if ::charset == NIL
	if ::tabPack
	for i=1 to ::lines
		n := len(::edbuffer[i])-len(ltrim(::edbuffer[i]))
		str := tabpack(padr(::edbuffer[i], n), ::tabSize)+ ;
			ltrim(::edbuffer[i])+"&\n"
		fwrite(nfile, str, len(str))
	next
	else
	for i=1 to ::lines
		str := ::edbuffer[i]+"&\n"
		fwrite(nfile, str, len(str))
	next
	endif
  else
	if ::tabPack
	for i=1 to ::lines
		//str := tabpack(::edbuffer[i], ::tabSize)
		n := len(::edbuffer[i])-len(ltrim(::edbuffer[i]))
		str := tabpack(padr(::edbuffer[i], n), ::tabSize)+ ;
			ltrim(::edbuffer[i])
		str := translate_charset(::__hostcharset, ::charset, str)+"&\n"
		fwrite(nfile, str, len(str))
	next
	else
	for i=1 to ::lines
		str := ::edbuffer[i]
		str := translate_charset(::__hostcharset, ::charset, str)+"&\n"
		fwrite(nfile, str, len(str))
	next
	endif
  endif
  fclose(nfile)
  ::updated:=.f.
  restore screen from scr
  //restScreen(0, 0, maxRow(), maxCol(), scr)
RETURN .t.
