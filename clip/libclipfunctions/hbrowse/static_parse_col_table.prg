static function parse_Col_Table(tag, align)
local curtag:=map()
	curtag:col := 1
	curtag:row := 1
	curtag:align := align
	if "COLSPAN"$tag:fields
		curtag:col := &(tag:fields:COLSPAN)
	endif
	if "ROWSPAN"$tag:fields
		curtag:row := &(tag:fields:ROWSPAN)
	endif
	if "ALIGN"$tag:fields
		curtag:align := upper(tag:fields:ALIGN)
	endif
return curtag
