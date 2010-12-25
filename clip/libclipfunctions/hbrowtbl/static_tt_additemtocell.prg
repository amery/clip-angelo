static function tt_addItemToCell(item, isTag)
	isTag := iif(isTag==NIL, .f., isTag)
	if isTag
		aadd(::Titems[::curRow][::curCol]:items, {clone(item), ::parseTag(item, .f.)})
	else
		::Titems[::curRow][::curCol]:addItem(item)
	endif
	if ::Titems[::curRow][::curCol]:minWidth > ::Tcolumns[::curCol]
		::Tcolumns[::curCol] := ::Titems[::curRow][::curCol]:maxWidth
	endif
return
