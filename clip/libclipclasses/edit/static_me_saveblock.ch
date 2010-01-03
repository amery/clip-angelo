static function me_saveBlock(nWin)
local obj := ::__checkWindow(@nWin)
local fd, r, s
	if !obj:strblock .and. !obj:rectblock
		alert([Block not marked!], "OK")
	else
		fd=filedialog(__CurrentDir, "*", ::__FileD)
		if !empty(fd)
			__CurrentDir := padr(fd, atr(PATH_DELIM, fd))
		endif
		r := file(fd)
		if r
			s:=alert([File;]+fd+[;already exists. Overwrite it?], {[Yes], [No]}, set("edit_colors_menu"))
		endif
		if !r .or. r .and. s == 1
			obj:saveBlock(fd, ::Opt:OCREATEBAK)
		endif
	endif
return .t.
