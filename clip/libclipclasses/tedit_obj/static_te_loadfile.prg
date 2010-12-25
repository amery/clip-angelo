static function te_loadFile(filename, lRefresh)
local nfile, scr, i
  lRefresh := iif(empty(lRefresh), .t., lRefresh)

  save screen to scr
  @ maxrow(), 0 say padr([Load file...], maxcol()) color "0/7"
  inkey()

  nfile:=fopen(filename)
  if nfile<0
     restore screen from scr
     return .f.
  endif

  *if !empty(fileseek(filename, FA_ARCHIVE+FA_VOLUME+FA_SYSTEM+FA_HIDDEN+FA_READONLY))
  *     alert(fileattrs()+';'+tostring(fileattr()))
  *endif
  i:=atr(PATH_DELIM,filename)
  if i!=0
	::path:=left(filename,i-1)
  endif
  ::fileName:=substr(filename,i+1)
  ::__nullInit()
  ::lines := 0
  if ::charset == NIL
	::__check_clipchs(::path)
  endif
  if ::charset != NIL
	while !fileeof(nfile)
		::lines ++
		/**/
		if ::lines > ::maxStrings
			::lines --
			alert([The size of file ;]+filename+[; most maximum size!;Last strings may be lost], [OK])
			exit
		endif
		/**/
		aadd(::edbuffer, translate_charset(::charset, ::__hostcharset, tabexpand(filegetstr(nfile,TE_MAX_LEN_STRING),::tabSize)) )
	enddo
  else
	while !fileeof(nfile)
		::lines ++
		/**/
		if ::lines > ::maxStrings
			::lines --
			alert([The size of file ;]+filename+[; most maximum size!;Last strings may be lost], [OK])
			exit
		endif
		/**/
		aadd(::edbuffer, tabexpand(filegetstr(nfile,TE_MAX_LEN_STRING),::tabSize) )
	enddo
  endif
  fclose(nfile)
  ::lines:=len(::edbuffer)
  ::line:=1
  ::pos:=1
  ::updated:=.f.
  restore screen from scr
  if lRefresh
	::refresh()
  endif
RETURN .t.
