static function me_setInsertMode(lMode)
	if lMode == NIL .or. valtype(lMode) != "L"
		lMode := !Set(_SET_INSERT)
	endif
	set( _SET_INSERT, lMode )
	if lMode
		setCursor( SC_INSERT )
	else
		setCursor( SC_NORMAL )
	endif
	::oMenu:_status("CHECKED", HASH_InsMode, lMode)
	::drawhead()
return  .t.
