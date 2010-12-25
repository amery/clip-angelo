static function parse_Title()
local s, t:=" "
	do while !::parser:empty()
		s=::parser:get()
		if valtype(s) == "O" .or. upper(s) == "/TITLE"
			exit
		endif
		t += s+" "
	enddo
	::doc:header := t
return len(t)
