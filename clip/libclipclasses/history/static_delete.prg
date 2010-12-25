static function delete(string)
local i, f:=.f.
	string := alltrim(string)
	for i:=1 to len(::history)
		if ::history[i] == NIL
			exit
		endif
		if alltrim(::history[i]) == string
			adel(::history, i)
			f := .t.
			exit
		endif
	next
return f
