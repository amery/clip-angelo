static function te_setNumStyle(st)
	st := iif(st==NIL, .t., st)
	if ::Nstyle == st
		return
	endif
	::Nstyle := st
	if st
		::__LNstyle := int(log10(::lines))+2
		::nLeft+=::__LNstyle
		::refresh()
	else
		::killNumStyle()
	endif
return
