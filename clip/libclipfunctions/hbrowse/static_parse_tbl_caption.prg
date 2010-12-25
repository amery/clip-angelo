static function parse_Tbl_Caption(tt)
local caption:=""
	do while !::parser:empty()
		tt := ::parser:get()
		if valtype(tt) == "C"
			caption += tt
		endif
		if valtype(tt)=="O" .and. tt:tagname == "/CAPTION"
			exit
		endif

	enddo
return caption
