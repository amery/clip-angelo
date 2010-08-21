static function tc_new_cell(tag)
	::type := hashstr(tag:tagName)
	if "COLSPAN"$tag:fields
		::colspan := &(tag:fields:COLSPAN)
	endif
	if "ROWSPAN"$tag:fields
		::rowspan := &(tag:fields:ROWSPAN)
		::Theight := ::rowspan
	endif
	if "ALIGN"$tag:fields
		::align := hashstr(upper(tag:fields:ALIGN))
	endif
return
static function tc_addCellItem(item)
local lastItem, arr, val
	aadd(::items, {item, len(item)})
	lastItem := atail(::items)
	::maxWidth += lastItem[2]
	arr := split(lastItem[1], " ")
	for val in arr
		if len(val) > ::minWidth
			::minWidth := len(val)
		endif
	next
return
