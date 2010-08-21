static function ci_fromIni(oIni)
	local s
	if valtype(oIni) != "O"
		return
	endif
	if !(oIni:className == "INIFILE")
		return
	endif

	s := oIni:getValue("INFO","LANGS")
	if !empty(s)
		if valtype(s)=="A"
			::langs := s
		elseif valtype(s)=="C"
			::langs := {}
			aadd(::langs,s)
		endif
	endif
	s := oIni:getValue("INFO","CHARSETS")
	if !empty(s)
		if valtype(s)=="A"
			::charsets := s
		elseif valtype(s)=="C"
			::charsets := {}
			aadd(::charsets,s)
		endif
	endif
	s := oIni:getValue("INFO","CRYPTS")
	if !empty(s)
		if valtype(s)=="A"
			::crypts := s
		elseif valtype(s)=="C"
			::crypts := {}
			aadd(::crypts,s)
		endif
	endif
return
