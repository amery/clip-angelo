static function te_findNext(Find, undo)
local ret:=.f., oldDirect
	undo:=iif(undo==NIL, .t., undo)
	if undo
		::writeundo(HASH_FIND, Find)//:fstring)
	endif
	oldDirect := Find:direct
	Find:direct := 1
	if !empty(Find:fstring)
		::gotoPos(::pos+1, ,.f.)
		ret := ::find(Find, .f.)
	endif

	Find:direct := oldDirect
	if undo
		::refresh()
	endif
return ret
