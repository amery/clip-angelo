static function db_refresh()
local bp,bl,l,i,j,dev, str, clr, pl:=.f., x, fnd, xi, elem, ch:=.f.
local dx

       ::line   := max(1, ::line)
       ::pos    := max(1, ::pos)
       if ::rowWin > ::nBottom-::nTop+1
	::rowWin := min(::nBottom-::nTop+1, ::line)
       elseif ::rowWin < ::nTop
	::rowWin := max(::rowWin, 1)
       else
	::rowWin := min(::line, max(::rowWin, 1))
       endif
       if ::colWin > ::nRight
	::colWin := min(::nRight, ::pos)
       elseif ::colWin < ::nLeft
	::colWin := max(::colWin, 1)
       else
	::colWin := min(::pos, max(::colWin, 1))
       endif

       fnd := iif(len(::findR)>0 .and. ::findR[1]!=NIL, .t., .f.)

       bl := ::line-::rowWin
       l  := ::nRight-::nLeft+1

       if fnd .and. !between(::findR[1], bl+1, bl+1+::nbottom-::ntop+1)
	   fnd := .f.
	   afill(::findR, NIL)
       endif

       dispbegin()
       for i=1 to ::nbottom-::ntop+1
	   if (bl+i) <= ::lines
		@ ::ntop+i-1, ::nLeft say replicate(" ", ::nRight-::nLeft+1) color ::__colors[1]
		for j=1 to len(::inbuffer[bl+i])
			elem := ::inbuffer[bl+i][j]
			str := ::getStrElem(elem, bl+i)

			if elem:nl<=(::pos - ::colWin)
				str=substr(str, ::pos-::colWin-elem:nl+2)
			endif
			if elem:nr>::pos-::colWin+(::nRight-::nLeft)
				str:= substr(str, 1, len(str)-(elem:nr-(::pos-::colWin+(::nRight-::nLeft)))+3)
			endif

			if (bl+i==::line) .and. elem:type == _GETB_ .and. between(::pos, elem:nl, elem:nr)
				if !elem:objit:hasFocus
					::item:=elem:objit
					asize(::item_area, 2)
					::item_area[1] := ::item:row
					::item_area[2] := ::item:col
					::item:col := max(elem:nl-::pos+::colWin, ::nLeft)
					::item:row := ::nTop+i-1
					::item_num := j
					::item_type := _GETB_
					::item:setFocus()
				endif
			elseif (bl+i==::line) .and. elem:type == _CHECK_ .and. between(::pos, elem:nl, elem:nr)
				if !elem:objit:Control:hasFocus
					::item := elem:objit
					asize(::item_area, 2)
					::item_area[2] := ::item:Control:col
					::item_area[1] := ::item:Control:row
					::item:Control:col := max(elem:nl-::pos+::colWin, ::nLeft)
					::item:Control:row := ::nTop+i-1
					@ ::ntop+i-1, ::item:Control:col say "[ ]" color elem:color
					::item_num := j
					::item_type := _CHECK_
					::item:Control:setFocus()
				endif
			elseif elem:type == _RADIO_ .and. between(::pos, elem:nl, elem:nr) .and. between(bl+i, elem:nt, elem:nb) .and. between(::line, elem:nt, elem:nb)
				if !elem:objit:Control:hasFocus
					::item := elem:objit
					if ::item:Control:bottom>::line+::nBottom-::rowWin
						::line := ::item:Control:bottom
						::rowWin := ::nBottom
						::refresh()
						exit
					endif

					if ::item:Control:top < ::line - ::rowWin
						::line := ::item:Control:top
						::rowWin := ::nTop+1
						::refresh()
						exit
					endif
					asize(::item_area, 2)
					::item_area[1] := {}
					::item_area[2] := {}
					asize(::item_area[1], ::item:Control:itemCount)
					asize(::item_area[2], ::item:Control:itemCount)
					for x=1 to ::item:Control:itemCount
						::item_area[1][x] := ::item:Control:__items[x]:row
						::item_area[2][x] := ::item:Control:__items[x]:col
						::item:Control:__items[x]:col := max(::item:Control:__items[x]:col-::pos+::colWin, ::nLeft)
						::item:Control:__items[x]:row := ::item:Control:top-bl+x-1
					next
					::item_num := j
					::item_type := _RADIO_
					::item:Control:setFocus()
				endif
			elseif elem:type == _GETTEXT_ .and. between(::pos, elem:nl, elem:nr) .and. between(bl+i, elem:nt, elem:nb) .and. between(::line, elem:nt, elem:nb)
				if !elem:objit:hasFocus
					::item:=elem:objit
					if ::item:edit:nTop< ::line-::rowWin+1
						::line := ::item:edit:nTop
						::rowWin := ::nTop
						ch := .t.//::refresh()
					endif
					if ::item:edit:nBottom>::line+::nBottom-::rowWin
						::line := ::item:edit:nBottom
						::rowWin := ::nBottom
						ch := .t.//::refresh()
					endif
					if ch
						::refresh()
					endif
					asize(::item_area, 4)
					::item_area[1] := ::item:edit:nTop
					::item_area[2] := ::item:edit:nLeft
					::item_area[3] := ::item:edit:nBottom
					::item_area[4] := ::item:edit:nRight
					::item:edit:nTop   := ::item:edit:nTop - bl
					::item:edit:nLeft  := max(::item:edit:nLeft-::pos+::colWin, ::nLeft)
					::item:edit:nBottom:= ::item:edit:nBottom - bl
					::item:edit:nRight := min(::item:edit:nRight-::pos+::colWin, ::nRight)

					::item_type := _GETTEXT_
					::item_num := j
					::item:setFocus()

				endif
			else
				if (bl+i==::line) .and. elem:type == _BUTTON_ .and. between(::pos, elem:nl, elem:nr)
					clr := ::__colors[4]+","+::__colors[2]
					::item:hasFocus := .t.
					::item:info := elem:info
					::item_type := _BUTTON_
					::item_num := j
					::item:userFunc := elem:fnc
				else
					if elem:type == _GETTEXT_ .and. elem:objit:hasFocus
						clr := ::item:colorSpec
					elseif elem:type == _RADIO_ .and. elem:objit:hasFocus
						clr := ::item:colorSpec
					else
						clr := elem:color+","+::__colors[2]
					endif
				endif
				dx := ::pos - ::colWin
				xi := max(elem:nl-dx, ::nLeft)
				if fnd .and. ::findR[1] == bl+i .and. between(::findR[2]-dx, xi, xi+len(str))
					@ ::ntop+i-1, ::nLeft+xi-1 say substr(str, 1, ::findR[2]-dx-xi) color clr
					@ ::ntop+i-1, ::nLeft+::findR[2]-dx-1 say substr(str, ::findR[2]-dx-xi+1, ::findR[3]) color ::__colors[4]
					@ ::ntop+i-1, ::nLeft+::findR[2]-dx+::findR[3]-1 say substr(str, ::findR[2]-dx-xi+1+::findR[3]) color clr
				else
					__sayCaption(::ntop+i-1, ::nLeft+xi-1, str, clr)
				endif
			endif
		next
	   endif

	   if (bl+i) > ::lines
	      @ ::ntop+i-1, ::nLeft say space(l) color ::__colors[1]
	   endif
       next
       //dispend()
       //devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
	if ::item_type == _GETB_
		::colWin := ::item:col+min(::item:pos, ::item:__winLen) - 1
		::pos := ::item_area[2]+min(::item:pos, ::item:__winLen) -1
	elseif ::item_type == _GETTEXT_
		::colWin := ::item:edit:nLeft+::item:edit:colWin-1
		::rowWin := ::item:edit:nTop+::item:edit:rowWin-1
		::line := ::item:edit:nTop + ::item:edit:rowWin-1
		::pos := ::item :edit:nLeft +  ::item:edit:colWin -1
	endif
	str:=padr("<"+alltrim(str(::line)+"><"+alltrim(str(::pos))+">"), ::leninfo, substr(B_DOUBLE, 6, 1))
	@ ::nT, 1 say str
	devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
	dispend()
return
