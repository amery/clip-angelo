static function te_loadBlock(filename, undo)
local nfile, i, path, s, arr, size, edb
	undo := iif(undo==NIL, .t., undo)
	filename := alltrim(filename)
	nfile := fopen(filename)
	if nfile < 0
		//messagep("Can't open file ;"+filename)
		return .f.
	endif
	if ::charset==NIL
		i:=atr(PATH_DELIM,filename)
		if i!=0
			path:=left(filename,i-1)
		endif
		::__check_clipchs(path)
	endif
	i := ::line
	if ::charset == NIL
		while !fileeof(nfile)
			::lines ++
			asize(::edbuffer, ::lines)
			ains(::edbuffer, i)
			::edbuffer[i] := tabexpand(filegetstr(nfile, TE_MAX_LEN_STRING), ::tabSize)
			i++
		enddo
	else
		while !fileeof(nfile)
			::lines ++
			asize(::edbuffer, ::lines)
			ains(::edbuffer,  i)
			::edbuffer[i] := translate_charset(::charset, ::__hostcharset,tabexpand(filegetstr(nfile, TE_MAX_LEN_STRING), ::tabSize), i)
			i++
		enddo
	endif
	fclose(nfile)
	if undo
		::writeundo(HASH_LOADBLOCK, {::line, i-1})
	endif
	::strblock := .t.
	::rectblock := .f.
	::koordblock[1] := ::line
	::koordblock[3] := i - 1
	::updated := .t.
	if undo
		::refresh()
	endif
RETURN .t.
