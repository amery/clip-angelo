function tcpServerLoadIni(iniFile,defIniFile)
	local oIni, s
	if empty(defIniFile)
		defIniFile := "clip-tcp.ini"
	endif
	if !empty(iniFile) .and. file(iniFile)
		oIni := iniFileNew(iniFile)
	endif
	if empty(oIni) .and. !empty(iniFile)
		? "Error: inifile not found:",iniFile
		? "Usage: "+startPath()+" <inifile_name>"
		?
		quit
	endif
	if empty(oIni)
		iniFile := cliproot()+PATH_DELIM+"etc"+PATH_DELIM+defIniFile
		if file(iniFile)
			oIni := iniFileNew(iniFile)
		endif

	endif
	if empty(oIni)
		iniFile := defIniFile
		if file(iniFile)
			oIni := iniFileNew(iniFile)
		endif

	endif
	if empty(oIni)
		oIni := iniFileNew()
		return oIni
	endif
	oIni:load()
	if !empty(oIni:error)
		s := [Error loading ini file:]+toString(iniFile)+":"+oIni:error
		? s
		outlog(s)
	endif
return  oIni
