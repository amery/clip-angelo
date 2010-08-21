static function parse_Ul()
local typ:="1", start:=1
	::new_clr(::__colors[1])
	aadd(::lists, {0, LISTDELTA+iif(len(::lists)==0, 0, ::lists[len(::lists)][LIST_DEEP]), 0})
	::width -= iif(len(::lists)>1, LISTDELTA, 0)
	::doc:marginLeft += iif(len(::lists)>1, LISTDELTA, 0)
return .t.
