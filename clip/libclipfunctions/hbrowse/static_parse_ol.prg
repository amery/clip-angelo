static function parse_Ol(tag)
local typ:="1", start:=1
	if "TYPE"$tag:fields
		typ := tag:fields:TYPE
	endif
	if "START"$tag:fields
		start := max(start, &(tag:fields:START))
	endif
	::new_clr(::__colors[1])
	aadd(::lists, {1, LISTDELTA+iif(len(::lists)==0, 0, ::lists[len(::lists)][LIST_DEEP]), start, typ})
	::width -= iif(len(::lists)>1, LISTDELTA, 0)
	::doc:marginLeft += iif(len(::lists)>1, LISTDELTA, 0)
return .t.
