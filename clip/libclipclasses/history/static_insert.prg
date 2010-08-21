static function insert(string)
	::delete(string)
	if len(::history)>1
		ains(::history, 1)
	else
		::setsize(10)
	endif
	::history[1] := string
return
