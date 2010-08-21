************************************
* format line/part
static function me_format(nWin, lFrmt)
local obj := ::__checkWindow(@nWin)
	lFrmt := iif(lFrmt==NIL, .T., lFrmt)
	if lFrmt
		obj:formatLine(::Opt:OAUTOMARGIN, ::Opt:OMARGINLEFT, ::Opt:OMARGINRIGHT, ::Opt:OTABSIZE, ::Opt:OHYPHEN)
	else
		obj:formatPart(::Opt:OAUTOMARGIN, ::Opt:OMARGINLEFT, ::Opt:OMARGINRIGHT, ::Opt:OTABSIZE, ::Opt:OHYPHEN)
	endif
return .t.
