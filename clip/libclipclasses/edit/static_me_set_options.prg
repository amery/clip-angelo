*************************************
static function me_set_options()
local str, arr, i, nfile
memvar obj, options
private obj, Options
	::Opt:=map()
	::Opt:OTABSIZE          := TE_TABSIZE
	::Opt:OAUTOSAVE         := TE_AUTO_SAVE
	::Opt:OTIMEAUTOSAVE     := TE_TIME_AUTO_SAVE
	::Opt:OCREATEBAK        := TE_CREATE_BAK
	::Opt:OMARGINLEFT       := TE_MARGIN_LEFT
	::Opt:OMARGINRIGHT      := TE_MARGIN_RIGHT
	::Opt:OSAVESTATUS       := TE_SAVE_STATUS
	::Opt:OLANGUAGE         := TE_LANGUAGE
	::Opt:OSTEPUNDO         := TE_STEP_UNDO
	::Opt:OHISTORY          := TE_HISTORY_SIZE
	::Opt:OAUTOMARGIN       := TE_AUTO_MARGIN
	::Opt:OHYPHEN           := TE_HYPHEN
	::Opt:OAUTOINDENT       := TE_AUTO_INDENT
	::Opt:OTABPACK          := set("edit_tabpack")
	::Opt:OMAXSTRINGS       := TE_MAXSTRINGS
	::Opt:OAUTOWRAP         := TE_AUTO_WRAP
	::Opt:OOPENFILES        := 1
	::Opt:FCASE             := .f.
	::Opt:FWORDONLY         := .f.
	::Opt:FREGEXP           := .f.
	::Opt:FALLW             := .f.
	::Opt:FDIRECT           := 1
	::Opt:FWHERE            := 1
	nfile := fopen(::__fileOpt)
	if nfile >= 0
		do while !fileeof(nfile)
			str := filegetstr(nfile, 1024)
			arr := split(str, ";")
			if len(arr)>0
				for i:= 1 to len(arr)
					&(arr[i])
				next
			else
				&str
			endif
		enddo
		fclose(nfile)
	endif
	//::Opt := Options
	o2self(::Opt, Options)
	set("edit_tabpack", ::Opt:OTABPACK)
	//::Opt:OTABPACK := set("edit_tabpack")
	::autoIndent := ::Opt:OAUTOINDENT
return .T.
