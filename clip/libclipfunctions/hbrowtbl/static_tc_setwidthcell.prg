static function tc_setWidthCell(width)
	if ::minWidth > width
		::width := ::minWidth
	else
		::width := width
	endif
return ::width
