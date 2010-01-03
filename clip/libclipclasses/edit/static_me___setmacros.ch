****************************
static function me___setMacros()
local sect, par, key, val, fl, i, fnc, hk, mnu, cmd
	if empty(::__iniMacro)
		return .f.
	endif
	sect := ::__iniMacro:Sections()
	for i=1 to len(sect)
		par := sect[i]
		val := ::__iniMacro:getValue(par, "UCOMMAND")
		if empty(val)
			loop
		endif
		fl := ::__iniMacro:getValue(par, "UFILE")
		if empty(fl) .or. !file(fl)
			loop
		endif

		load(fl)

		if "UCOMMAND" $ ::__iniMacro:data[par] .and. (::__iniMacro:data[par]:UCOMMAND != NIL)
			cmd := hashStr(::__iniMacro:getValue(par, "UCOMMAND"))
			fnc := ::__iniMacro:getValue(par, "UFUNC")
			if empty(fnc) .or. !isFunction(fnc)
				loop
			endif
			hk := keycodeByName(::__iniMacro:getValue(par, "UHOTKEY"))
			if !(hk $ ::__macroKeys)
				::__macroKeys[hk] := map()
			endif
			::__macroKeys[hk]:func := fnc
			::__macroKeys[hk]:allWins := ::__iniMacro:getValue(par, "UALLWINS")
			::__macroKeys[hk]:command := cmd

			if empty(::__macroKeys[hk]:allWins)
				::__macroKeys[hk]:allWins := .f.
			endif

			if !(cmd $ ::__macroCmd)
				::__macroCmd[cmd] := map()
			endif
			::__macroCmd[cmd]:func := fnc
			::__macroCmd[cmd]:command := cmd
			::__macroCmd[cmd]:allWins := ::__macroKeys[hk]:allWins

			mnu := ::__iniMacro:getValue(par, "UMENUSTR")
			if !empty(mnu)
				::__addMacroToMenu(par, mnu, fnc, cmd, hk)
			endif
		endif
	next
return .t.
