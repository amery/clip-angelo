function tcpServerLoadIniModules(oIni,type)
	local i,j,tmp,dir,file,bl,mFuncs := {}
	if valtype(oIni)=="O" .and. oIni:className == "INIFILE"
	else
		return
	endif
	if empty(type)
		type := "TCP"
	endif
	tmp := oIni:getValue(type,"MOD-INI")
	if empty(tmp)
		return
	endif
	tmp := strtran(tmp,"$CLIPROOT",cliproot())
	dir:=directory(tmp+PATH_DELIM+"*.po")
	? "Found "+alltrim(str(len(dir)))+"init modules in directory:"+tmp
	for i=1 to len(dir)
		load(tmp+PATH_DELIM+dir[i][1],mFuncs)
		for j=1 to len(mFuncs)
			? "Run init module:"+mFuncs[j]+":"
			clip(mFuncs[j],oIni)
		next
		mFuncs := {}
	next
return
