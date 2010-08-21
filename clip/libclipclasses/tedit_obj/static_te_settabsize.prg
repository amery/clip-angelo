static function te_setTabSize(tabsize)
local i, n, str, ts
	::tabsize := iif(tabsize==NIL, ::tabsize, tabsize)
	ts := ::tabSize
	aeval(::edbuffer, {|item| n:=len(item)-len(ltrim(item)), ;
		str:=tabpack(padr(item, n), ts)+ltrim(item), ;
		item := tabexpand(str, tabsize)	})
	::refresh()
return
