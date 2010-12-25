static function me_saveWins()
local k, i, ret := .t.
	if ::nWins < 1
		return ret
	endif
	k := 0
	for i=1 to ::nWins
	if ::tobj[i] == NIL
		loop
	endif
	wselect(::tobjinfo[i]:curwin)
	if ::tobj[i]:updated
		k := alert([File;]+::tobjinfo[i]:name+[;not saved. Save it?],{[Yes],[No],[Cancel] })
		if k==1
			::saveFile(, i)
		elseif k==3 .or. k==0
			ret := .f.
			exit
		endif
		if empty(::tobjinfo[i]:name) .and. ::tobj[i]:updated
			::removeWindow(@i)
			if i == ::curwin
				::__setFocus(::curwin - 1)
			endif
		endif
	else
		if empty(::tobjinfo[i]:name)
			::removeWindow(@i)
			if i == ::curwin
				::__setFocus(::curwin - 1)
			endif
			::curwin --
		endif
	endif
	wclose()

next
::curwin := ::__setFocus(max(0, ::curwin))

return ret
*