***************************
static function me_writestatus()
local str:="", nfile:="", buflen:=1024, i, _e:="&\n"
private len

	nfile := fcreate(::__fileStatus)
	if nfile < 0
		return .f.
	endif
	str := var2str(::curwin)+_e
	fwrite(nfile, str, len(str))
	str := var2str(::__FileD:history)+_e
	fwrite(nfile, str, len(str))
	str := var2str(::Find:fhistory:history)+_e
	fwrite(nfile, str, len(str))
	str := var2str(::Find:rhistory:history)+_e
	fwrite(nfile, str, len(str))
	for i := 1 to ::nWins
		::tobjinfo[i]:pos := ::tobj[i]:pos
		::tobjinfo[i]:line := ::tobj[i]:line
		::tobjinfo[i]:colwin := ::tobj[i]:colWin
		::tobjinfo[i]:rowwin := ::tobj[i]:rowWin
	next
	str := var2str(::tobjinfo)
	fwrite(nfile, str, len(str))

	fclose(nfile)
return .t.
