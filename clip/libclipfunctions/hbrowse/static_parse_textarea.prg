static function parse_TextArea(tag, width)
local item:=map(), ind, clr, str:="", s, vt
	ind := len(::form)
	item:name := ""
	item:cols := 0
	item:rows := 0
	item:wrap := "OFF"
	if "NAME"$tag:fields
		item:name := upper(tag:fields:NAME)
	endif
	if "COLS"$tag:fields
		item:cols := &(tag:fields:COLS)
	endif
	if "ROWS"$tag:fields
		item:rows := &(tag:fields:ROWS)
	endif
	if "WRAP"$tag:fields
		item:wrap := upper(tag:fields:WRAP)
	endif
	clr:=COLOR_TEXTAREA
	if item:cols==0
		item:cols := max(item:cols, int(::width/3))
	endif
	if item:rows==0
		item:rows := max(item:rows, int(::rows/3))
	endif
	do while !::parser:empty()
		s := ::parser:get()
		vt := valtype(s)
		if vt=="O"
			exit
		endif
		if vt == "C"
			str += s+"&\n"
		endif
	enddo
	str := left(str, len(str)-1)
	::doc:GetText(::doc:n_Line,::doc:n_Pos,::doc:n_Line+item:rows,::doc:n_Pos+item:cols,str,clr,,,,item:name)
return item:cols
