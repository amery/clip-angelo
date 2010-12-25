static function get_hitTest(mrow,mcol)
	if ::row != mrow
		return HTNOWHERE
	endif
	if mcol >= ::col .and. mcol <= ::col+::__winlen
		return HTCLIENT
	endif
return HTNOWHERE
