static function parse_Li(tag)
local typ:="1", value:=0, item
	if len(::lists) == 0
		return 0
	endif
	item := atail(::lists)
	typ := iif(len(item)>=4,item[LIST_VID],"")
	if "TYPE"$tag:fields
		typ := tag:fields:TYPE
	endif
	if "VALUE"$tag:fields
		value := tag:fields:VALUE
	endif
	if item[LIST_TYPE] == 0 // unordered lists
		::str := "-":padl(item[LIST_DEEP])
		return len(::str)
	endif
	do case
	case typ == "A"
		::str := (upper(chr(97+item[LIST_START]-1))+"."):padl(item[LIST_DEEP])
	case typ == "a"
		::str := (chr(97+item[LIST_START]-1)+"."):padl(item[LIST_DEEP])
	case typ == "I"
	case typ == "i"
	otherwise
		::str := (alltrim(toString(item[LIST_START]))+"."):padl(item[LIST_DEEP])
	endcase
	item[LIST_START] ++
return len(::str)
