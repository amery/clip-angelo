static function db_drawBox(vref)
local str
	vref := iif(vref==NIL, .t., vref)
	@ ::nT, ::nL clear to ::nB, ::nR
	//@ ::nT, ::nL, ::nB, ::nR box B_DOUBLE
	@ ::nT, ::nL TO ::nB, ::nR DOUBLE
	str:=padr("<"+alltrim(str(::line)+"><"+alltrim(str(::pos))+">"), ::leninfo, substr(B_DOUBLE, 6, 1))
	dispbegin()
	@ ::nT, 1 say str
	@ ::nT, (maxcol()-len(::header))/2 say ::header
	devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
	dispend()
	setcursor(1)
	if vref
		::refresh()
	endif
return
