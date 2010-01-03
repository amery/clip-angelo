static function me_getstatus()
local str, arr, nfile, i, buflen:=1024
private obj
	if !File(::__fileStatus)
		return .f.
	endif
	nfile := fopen(::__fileStatus)
	str := fget(nfile, buflen)
	::curwin := str2var(str)
	//? 'get curwin'
	str := fget(nfile, buflen)
	::__FileD:set(str2var(str))
	//? 'get fileD'
	str := fget(nfile, buflen)
	::Find:sethistfind(str2var(str))
	//? 'get Find'
	str := fget(nfile, buflen)
	::Find:sethistrepl(str2var(str))
	//? 'get Find repl'
	str := fget(nfile, buflen)
	::tobjinfo := str2var(str)
	::nWins := len(::tobjinfo)

	//? 'get info'
	fclose(nfile)

return .t.
