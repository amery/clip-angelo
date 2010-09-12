**************************************

static function me_saveFile(asFile, nWin)
local fd, f, al:=0, crbak
   crbak := ::Opt:OCREATEBAK
   asFile := iif(asFile==NIL, .f., asFile)
   nWin := iif(nWin==NIL, ::curwin, nWin)

   if empty(::tobjinfo[nWin]:name) .or. asFile
	fd=filedialog(__CurrentDir, "*", ::__FileD)
	if !empty(fd)
		__CurrentDir := padr(fd, atr(PATH_DELIM, fd))
	endif
	if empty(fd)
		return .f.
	endif
	f := file(fd)
	if f
		al:=alert([File;]+fd+[;already exists. Overwrite it?], {[Yes], [No]}, set("edit_colors_menu"))
	endif
	if al == 3
		return .f.
	endif
	if !f .or. f .and. al == 1
		if ::tobj[nWin]:saveFile(fd, crbak)
			::tobjinfo[nWin]:name := fd
			::drawhead()
		else
			alert([Can't save file ;]+fd)
		endif
	endif
   else
	fd := ::tobjinfo[nWin]:name
	::tobj[nWin]:saveFile(fd, crbak)
   endif
return .t.
