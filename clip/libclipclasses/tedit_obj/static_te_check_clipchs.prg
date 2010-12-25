static function te_check_clipchs(path)
local file:=path+"/.clipcharset"
	if file(file)
		::charset := memoread(file)
		::charset := charrepl("&\t&\n", ::charset, "  ")
		::charset := alltrim(::charset)
	endif
return ::charset
