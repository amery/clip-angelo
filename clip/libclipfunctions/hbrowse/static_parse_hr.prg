static function parse_Hr(tag)
local align:="CENTER", size:=::width
	if "ALIGN"$tag:fields
		align := upper(tag:fields:ALIGN)
	endif
	if "SIZE"$tag:fields
		size := tag:fields:SIZE
	endif

	if align == "LEFT"
		::str := padl(replicate(DELIMITER, size), ::width)
	elseif align == "RIGHT"
		::str := padr(replicate(DELIMITER, size), ::width)
	else
		::str := padc(replicate(DELIMITER, size), ::width)
	endif
	::newStr := .t.
return size
