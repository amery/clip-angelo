static function tt_setCell(item, isTag)
	::Titems[::curRow][::curCol]:addItem(item, isTag)

	if ::Titems[::curRow][::curCol]:minWidth > ::Tcolumns[::curCol]
		::Tcolumns[::curCol] := ::Titems[::curRow][::curCol]:maxWidth
	endif
return
