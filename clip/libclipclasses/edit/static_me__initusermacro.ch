****************************
/* get ini file and set user macroses if it specified */
static function me__initUserMacro(iniFileName)
local i, home
	home := getEnv("HOME")
	if iniFileName == NIL
		i := strtran(startpath(), ".exe", "")
		i := substr(i, atr(PATH_DELIM, i)+1)
		iniFileName := home+PATH_DELIM+".clip"+PATH_DELIM+i+".ini"
		if !file(iniFileName)
			iniFileName := cliproot()+PATH_DELIM+'etc'+PATH_DELIM+i+'.ini'
		endif
	endif
	if file(iniFileName)
		::__iniMacro := iniFileNew(iniFileName)
		::__iniMacro:Load()
		::__setMacros()
		return .T.
	endif
return .F.
