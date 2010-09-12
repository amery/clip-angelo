static function edit_hitTest(mrow,mcol)
	if mrow < ::row .or. mrow > ::nBottom
		return HTNOWHERE
	endif
	if mcol < ::col .or. mcol > ::nRight
		return HTNOWHERE
	endif
return HTCLIENT
