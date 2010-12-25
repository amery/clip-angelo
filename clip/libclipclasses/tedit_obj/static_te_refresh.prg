static function te_refresh()
local bp,bl,l,i,dev, str, str1, block, min, max, fnd, clr_st

	::line   := max(1, ::line)
	::pos    := max(1, ::pos)

	::rowWin := min(min(max(1,::rowWin),::nBottom-::nTop+1),::line)
	::colWin := min(min(max(1,::colWin),::nRight-::nLeft+1),::pos)

       if ::mkblock
	   ::koordblock[3] := ::line
	   ::koordblock[4] := ::pos
       endif
       if ::lines > ::maxStrings
		alert([The size of file is a most maximum size!;Last strings may be lost; Please check MAXSTRINGS options], [OK])
		::lines := ::maxStrings
		asize(::edbuffer, ::lines)
       endif

       fnd := iif(::__findR[1]!=NIL, .t., .f.)

       bl := ::line-::rowWin
       bp := ::pos-::colWin
       l  := ::nRight-::nLeft+1//iif(::nLeft<1, 1, 0)
       dispbegin()
       block :=  ::strblock .or. ::rectblock

       if ::rectblock
	   if ::koordblock[2]<=::koordblock[4]
	       min := ::koordblock[2]-bp
	       max := ::koordblock[4]-bp
	   else
	       min := ::koordblock[4]-bp
	       max := ::koordblock[2]-bp
	   endif
	   min := max(min, 1)
	   max := max(max, 0)
       endif
       if fnd .and. !between(::__findR[1], bl+1, bl+1+::nbottom-::ntop+1)
	   fnd := .f.
	   afill(::__findR, NIL)
       endif
       for i=1 to ::nbottom-::ntop+1
	   if (bl+i) <= ::lines
	      str=padr(substr(::edbuffer[bl+i],bp+1,bp+l),l)
	      if (bl+i)$::highLightColor
		@ ::ntop+i-1, ::nLeft say str color ::__colors[::highLightColor[bl+i][1]]
		clr_st := ::__colors[::highLightColor[bl+i][1]]
	      elseif !block .or. (block .and. !between(bl+i, ::koordblock[1], ::koordblock[3]))
		   clr_st := ::__colors[1]
		   if fnd .and. ::__findR[1] == bl+i
			@ ::ntop+i-1, ::nLeft say substr(str, 1, ::__findR[2]-1) color ::__colors[1]
			@ ::ntop+i-1, ::nLeft+::__findR[2]-1 say substr(str, ::__findR[2], ::__findR[3]) color ::__colors[4]
			@ ::ntop+i-1, ::nLeft+::__findR[2]+::__findR[3]-1 say substr(str, ::__findR[2]+::__findR[3]) color ::__colors[1]
		   else
			@ ::ntop+i-1, ::nLeft say str color ::__colors[1]
		   endif
	      else
		   if ::strblock
			@ ::ntop+i-1, ::nLeft say str color ::__colors[3]
			clr_st := ::__colors[3]
		   else

			if max >0
				dev := max(0, min)
				str1=substr(str, 1, dev-1)
				@ ::ntop+i-1, ::nLeft say str1 color ::__colors[1]
				str1=substr(str, dev, max-min+1)
				@ ::ntop+i-1, ::nLeft+max(dev-1, 0) say str1 color ::__colors[3]
				dev += max-min+1
				str1=substr(str, dev)
				@ ::ntop+i-1, ::nLeft+max(dev-1, 0) say str1 color ::__colors[1]
				clr_st := ::__colors[1]
			else
				@ ::ntop+i-1, ::nLeft say str color ::__colors[1]
				clr_st := ::__colors[1]
			endif
		   endif
	      endif
	   endif
	   if (bl+i) == ::lines+1
	      if ::lEofString
		@ ::ntop+i-1, ::nLeft say padr(::eofString,l) color ::__colors[2]
	      endif
	      clr_st := ::__colors[2]
	   endif
	   if ::Nstyle .and. (bl+i) < ::lines+iif(::lEofString,1,0)
		@ ::ntop+i-1, ::nLeft-::__LNstyle say str(bl+i, ::__LNstyle-1, 0)+"�" color clr_st
	   endif
	   if (bl+i) > ::lines+iif(::lEofString,1,0)
	      @ ::ntop+i-1, ::nLeft-::__LNstyle say space(l) color ::__colors[1]
	   endif
       next
       devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
       dispend()
return
