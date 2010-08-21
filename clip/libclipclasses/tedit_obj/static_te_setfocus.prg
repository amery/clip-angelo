static function te_setFocus(lFocus)
	lFocus := iif(lFocus==NIL, !::InFocus, lFocus)
	::Infocus := lFocus
return ::InFocus
