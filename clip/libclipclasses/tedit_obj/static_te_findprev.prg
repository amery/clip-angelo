static function te_findPrev(Find, undo)
local ret:=.f., oldDirect
	undo:=iif(undo==NIL, .t., undo)
	if undo
		::writeundo(HASH_FIND, Find)//:fstring)
	endif

	oldDirect := Find:direct
	Find:direct := 2
	if !empty(Find:fstring)
		::gotoPos(::pos-1, ,.f.)
		ret := ::find(Find)
	endif

	Find:direct := oldDirect
	if undo
		::refresh()
	endif
return ret
