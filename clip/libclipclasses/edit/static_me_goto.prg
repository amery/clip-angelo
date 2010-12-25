***************************
static function me_goto()
local scr, top, bottom, left, right, str, getstr, oldcolor, getobj, getlist:={}
local nKey, newline

save screen to scr
oldcolor=setcolor(set("edit_colors_menu"))

str := [Line ]
top := maxrow()/2 - 1
bottom := top + 2
left := maxcol()/2 - len(str) - 10
right := left + len(str) + 10
@ top, left clear to bottom, right
@ top, left to bottom, right
::__Hist:Lrow := top + 2
::__Hist:Lcol := left +len(str) +2
::__Hist:Rrow := top + 7
::__Hist:Rcol := right - 2

getstr := padl(val(::__Hist:first()), 8)

@ top+1, left + 1 say str// get getstr picture "@kS89"
getobj := getnew(top+1, left + len(str) +1, {|_1| iif(_1==NIL, padr(getstr, 10),getstr:=_1)}, "getstr", "@kS8", setcolor())
getobj:setFocus()
do while .t.
	nKey := Inkey(0)
	do case
	case nKey == K_ESC
		newline := 0
		exit
	case nKey == K_DOWN
		newline := ::__Hist:run()
		if !empty(newline)
			getobj:varPut(newline)
			getobj:assign()
		endif
		getobj:setFocus()
		getobj:home()
	case nKey == K_ENTER .or. nKey == K_PGDN
		 getobj:assign()
		 newline := getobj:varGet()
		 exit
	case nKey == K_HOME
		getobj:home()
	case nKey == K_END
		getobj:end()
	case nKey == K_DEL
		getobj:delete()
	case nKey == K_BS
		getobj:backspace()
	otherwise
		if nkey>=32 .and. nkey<=256
			if set(_SET_INSERT)
				getobj:Insert(chr(nKey))
			else
				getobj:overStrike(chr(nKey))
			endif
		endif
	endcase
enddo
setcolor(oldcolor)
restore screen from scr
setcursor(1)

if !empty(newline)
	::__Hist:insert(newline)
	if !::tobj[::curwin]:gotoLine(val(newline), .t.)
		alert([Bad line number!], "OK")
		::__Hist:delete(newline)
	endif
endif

return newline
