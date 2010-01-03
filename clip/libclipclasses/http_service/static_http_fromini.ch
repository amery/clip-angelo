
	/* install values from ini-file */
/*****************************************/
static function HTTP_fromIni(obj,oIni,vHost)
	local tmp,i,j,a,b,allKeys,_vHost
	if valtype(oIni)=="O" .and. oIni:className == "INIFILE"
	else
		return
	endif
	if empty(vHost)
		_vHost := "HTTP"
	else
		_vHost := vHost
	endif
	tmp = oIni:getValue(_vHost,"ADMIN")
	if !empty(tmp)
		obj:admin := tmp
	endif
	tmp = oIni:getValue(_vHost,"DOCROOT")
	if !empty(tmp)
		tmp := strtran(tmp,"$CLIPROOT",cliproot())
		obj:DocRoot := tmp
	endif
	tmp = oIni:getValue(_vHost,"CGI-BIN")
	if !empty(tmp)
		tmp := strtran(tmp,"$CLIPROOT",cliproot())
		obj:cgiBin := tmp
	endif
	tmp = oIni:getValue(_vHost,"MOD-CLIP")
	if !empty(tmp)
		tmp := strtran(tmp,"$CLIPROOT",cliproot())
		obj:modClip := tmp
	endif
	tmp = oIni:getValue(_vHost,"MOD-CACHE")
	if !empty(tmp)
		tmp := strtran(tmp,"$CLIPROOT",cliproot())
		obj:modCache := tmp
	endif
	if !empty(vHost)
		return
	endif
	tmp := oIni:getValue("HTTP","DATEFORMAT")
	if !empty(tmp)
		obj:dateFormat := tmp
	endif
	tmp := oIni:getValue("HTTP","SSI")
	if !empty(tmp)
		if valtype(tmp)=="C"
			tmp := split(tmp,"[ \t]")
		endif
		if valtype(tmp)=="A"
			obj:ssi := aclone(tmp)
		endif
	endif
	tmp := oIni:getValue("HTTP","REQUESTTIMEOUT")
	if valtype(tmp) == "N"
		obj:nIoTimeOut := tmp
	endif
	tmp := oIni:getValue("FILE-EXT-ICONS")
	if !empty(tmp)
		for i in tmp keys
			obj:fileicons[i] := tmp[i]
		next
	endif
	tmp := oIni:getValue("MIME-TYPE-ICONS")
	if !empty(tmp)
		for i in tmp keys
			obj:mimeicons[i] := tmp[i]
		next
	endif
	allkeys := oIni:getValue("HTTP-ALIASES")
	for i in allkeys
		if empty(i)
			loop
		endif
		j := at("->",i)
		if j <= 0
			loop
		endif
		a := alltrim(left(i,j-1))
		b := alltrim(substr(i,j+3))
		if empty(a) .or. empty(b)
			loop
		endif
		if !(right(a,1) =="/")
			a += "/"
		endif
		if !(right(b,1) =="/")
			b += "/"
		endif
		aadd(obj:aliases,{a,b})
	next
return
