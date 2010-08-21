static function hp_get(s)
	local ret:=NIL
	if !empty(::tags)
		ret:=::tags[1]
		adel(::tags,1)
		asize(::tags,len(::tags)-1 )
	endif
return ret
