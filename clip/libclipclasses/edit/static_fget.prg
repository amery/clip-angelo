*************************************
static function fget(nfile, buflen)
local ret:="", str
	str := filegetstr(nfile, buflen)
	if len(str) != buflen
		return str
	endif
	do while len(str) == buflen .and. !fileeof(nfile)
		ret += str
		str := filegetstr(nfile, buflen)
	enddo
return ret
