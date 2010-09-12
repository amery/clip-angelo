static function get()
local k, r, s, ret, i, nkey:=1, length, focus:=1
local a, x, col2, col1, line:=1, obj[8], str, replstr, oldcolor

k := int(2*maxcol()/3)
r := int(2*maxrow()/3)
oldcolor := setcolor()
s := setcolor(::__colors[8])
str := iif(::fstring==NIL,space(0),::fstring)
replstr := iif(::rstring==NIL,space(0),::rstring)

wopen(int((maxrow()-r)/2), int((maxcol()-k)/2), int((maxrow()-r)/2)+r, int((maxcol()-k)/2)+k)
wbox()
::fhistory:Lrow := 2
::fhistory:Lcol := 2
::fhistory:Rrow := 7
::fhistory:Rcol := maxcol()-2
::rhistory:Lrow := 4
::rhistory:Lcol := 2
::rhistory:Rrow := 9
::rhistory:Rcol := maxcol()-2
clear screen
col1 := maxcol()/2-8
col2 := maxcol()/2
//str := padr(str, length)
s := setcolor(::colorSpec)

if ::replace
	@ -1, (maxcol()-len([ Find & replace ]))/2 say [ Find & replace ]
	length := maxcol()-12
	@ line, 1 say padl([Find:], 10)
	//obj[focus] := getnew(line++, 11,{|_1| iif(_1==NIL, padr(str, 256),str:=_1)}, "str", "@kS"+alltrim(str(length)), ::__colors[3])
	obj[focus] := getnew(line++, 11,{|_1| iif(_1==NIL, padr(str, 256),str:=_1)}, "str", "@kS"+alltrim(str(length)), ::colorSpec)
	obj[focus]:varPut(str)
	obj[focus]:assign()
	line++
	focus++
	@ line, 1 say padl([Replace:], 10)
	obj[focus] := getnew(line++, 11,{|_1| iif(_1==NIL, padr(replstr, 256),replstr:=_1)}, "replstr", "@kS"+alltrim(str(length)), ::colorSpec)
	obj[focus]:varPut(replstr)
	obj[focus]:assign()
	line++
else
	@ -1, (maxcol()-len([ Find ]))/2 say [ Find ]
	length := maxcol()-2
	//obj[focus] := getnew(line++, 1,{|_1| iif(_1==NIL, padr(str, 256),str:=_1)}, "str", "@kS"+alltrim(str(length)), ::__colors[3])
	obj[focus] := getnew(line++, 1,{|_1| iif(_1==NIL, padr(str, 256),str:=_1)}, "str", "@kS"+alltrim(str(length)), ::colorSpec)
	obj[focus]:varPut(str)
	obj[focus]:assign()
	line++
	focus++
endif
x:=line
@ line, 1 clear to line+5, col2-2// color ::colorSpec
//@ line, 1, line+5, col2-2 box B_SINGLE color ::colorSpec
@ line, 1 TO line+5, col2-2 color ::colorSpec
@ line, 3 say [Options] color ::colorSpec
line++
focus++

obj[focus++] := _checkbox_(iif(::case==NIL, .f., ::case), padr([Case-sensitive], col1), "",::colorSpec,,,,,line++,2)
obj[focus++] := _checkbox_(iif(::wordonly==NIL, .f., ::wordonly), padr([Word only], col1), "",::colorSpec,,,,,line++,2)
obj[focus++] := _checkbox_(iif(::regexp==NIL, .f., ::regexp), padr([Reg. expression], col1), "",::colorSpec,,,,,line++,2)
obj[focus++] := _checkbox_(iif(::allw==NIL, .f., ::allw), padr([All windows], col1), "",::colorSpec,,,,,line++,2)
//line := 4
line := x
a := {}
aadd(a, radioButton(line++, col2+1, padr([Forward], col2-6)))
aadd(a, radioButton(line++, col2+1, padr([Backward], col2-6)))
aadd(a, radioButton(line++, col2+1, padr([From begin], col2-6)))
obj[focus++] := _radiogrp_(line-4, col2, line, maxcol()-1, iif(::direct==NIL, 1, ::direct), a,[Direction],,::colorSpec)
line+=2
a := {}
aadd(a, radioButton(line++, col2+1, padr([Global], col2-6)))
aadd(a, radioButton(line++, col2+1, padr([In block], col2-6)))
obj[focus++] := _radiogrp_(line-3, col2, line, maxcol()-1, iif(::where==NIL, 1, ::where), a,[Where],,::colorSpec)

obj[1]:setFocus()
focus := 1

while nkey != 0
	nkey := inkey(0)
	do case
	case nkey == K_ESC
		ret := .f.
		nkey := 0
	case nkey == K_ENTER
		nkey := 0
		s := rtrim(obj[1]:buffer) //varGet()
		if empty(s)
			ret := .f.
			exit
		endif
		::fhistory:insert(s)

		::fstring  := s //string for find
		::case     := obj[3]:buffer // case sensitive .f.
		::wordonly := obj[4]:buffer // word only
		::regexp   := obj[5]:buffer // regular expression .f.
		::allw     := obj[6]:buffer // all window .f.
		::direct   := obj[7]:value // direct 1/2/3
		::where    := obj[8]:value // in block .f.

		if ::replace
			s := rtrim(obj[2]:buffer) //varGet()
			::rhistory:insert(s)
			::rstring := s //string for replace
		endif
		ret := .t.
	case nkey == K_LEFT
		obj[focus]:left()
	case nkey == K_RIGHT
		obj[focus]:right()
	case nkey == K_HOME
		obj[focus]:home()
	case nkey == K_END
		obj[focus]:gotopos(len(alltrim(obj[focus]:varGet()))+1)
	case nkey == K_DOWN
		if focus == 1 .or. focus == 2//get
			if focus == 2
				s := ::rhistory:run()
				if !empty(s)
					::rhistory:insert(s)
				else
					s := obj[focus]:varGet()
				endif
			else
				s := ::fhistory:run()
				if !empty(s)
					::fhistory:insert(s)
				else
					s := obj[focus]:varGet()
				endif
			endif
			obj[focus]:setFocus()
			obj[focus]:killFocus()
			obj[focus]:varPut(s)
			//obj[focus]:assign()
			obj[focus]:setFocus()
			obj[focus]:gotopos(len(alltrim(obj[focus]:varGet()))+1)
		else
			//if focus < 6 .or. rep .and. focus < 7 //len(obj)-1
			if focus < 7
				obj[focus]:killFocus()
				focus++
				if focus > 8
					focus := 1
				endif
				obj[focus]:setFocus()
			else
				obj[focus]:nextItem()
			endif
		endif
	case nkey == K_UP
		if focus < 7
			obj[focus]:killFocus()
			focus --
			if obj[2] == NIL .and. focus == 2
				focus --
			endif
			if focus < 1
				obj[1]:assign()
				focus := 8
			endif
			obj[focus]:setFocus()
		else
			obj[focus]:prevItem()
		endif
	case nkey == K_TAB
		if focus == 1 .or. focus == 2
			obj[focus]:assign()
		endif
		obj[focus]:killFocus()
		if between(focus, 3, 6)
			focus := 7
		else
			focus++
			if obj[2] == NIL .and. focus == 2
				focus ++
			endif
		endif
		if focus > 8
			focus := 1
		endif
		obj[focus]:setFocus()
	case nkey == K_DEL
		if focus == 1 .or. focus == 2
			obj[focus]:delRight()
		endif
	case nkey == K_BS
		if focus == 1 .or. focus == 2
			obj[focus]:backSpace()
		endif
	otherwise
	    if nkey == 32 .and. (focus != 1 .and. focus != 2)
		if between(focus, 3, 6)
			obj[focus]:select()
		endif
	    else
		if nkey>=32 .and. nkey<=256
			obj[focus]:Insert(chr(nkey))
		endif
	    endif
	endcase
enddo

setcolor(s)
wclose()
setcolor(oldcolor)
return ret
