static function COBRA_receiveLine()
	local i
	::errno := 0
	::error := ""
	while (i := atl("&\n", @::buffer) ) <=0
		::receive()
		if ::errno > 0 .and. ::errno != 11 /* server is busy */
			return .f.
		endif
	end
	::cLine := left(@::buffer,i-1)
	::buffer := substr(@::buffer,i+1)
return .t.
