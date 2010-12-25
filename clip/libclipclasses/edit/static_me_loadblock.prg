************************************
* load block from file
static function me_loadBlock(nWin)
local obj := ::__checkWindow(@nWin), fd, r
	fd=filedialog(__CurrentDir, "*", ::__FileD)
	if !empty(fd)
		__CurrentDir := padr(fd, atr(PATH_DELIM, fd))
	endif
	r := file(fd)
	if !r
		alert([File;]+fd+[;not found!],"OK", set("edit_colors_menu"))
	else
		if !obj:loadBlock(fd)
			alert([Can't open file;]+fd, "OK")
		endif
	endif
return .t.
