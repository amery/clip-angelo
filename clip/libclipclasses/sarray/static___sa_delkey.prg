static function __SA_delKey(key,value)
	local lExact:=set(_SET_EXACT,.t.)
	local nLen:=len(::items), ret:=.f.
	local nPos:=-1,bPos:=1, ePos:=nLen
	::error:=""
	if key == NIL .or. value == NIL
		::error:=[Unknown values for key or data]
		set(_SET_EXACT,lExact)
		return .f.
	endif
	if nLen==0
	elseif key <  ::items[1][1]
	elseif key >= ::items[nLen][1]
	else
		nPos:=int(nLen/2)
		do while .t.
			if key>=::items[nPos][1]
				bPos=nPos-1
				nPos:=bPos+int((ePos-bPos)/2)
			else
				ePos=nPos+1
				nPos:=bPos+int((ePos-bPos)/2)
			endif
			if ePos-bpos < 5
				for nPos=bPos to ePos
				    if key==::items[nPos][1] .and. value==::items[nPos][2]
					exit
				    endif
				next
				exit
			endif
		enddo
	endif
	if nPos>0 .and. nPos<=nLen .and. key==::items[nPos][1] .and. value==::items[nPos][2]
		adel(::items, nPos)
		asize(::items, nlen-1)
		return .t.
	endif
	set(_SET_EXACT,lExact)
	::error:=[Key value not found in sorted array]
return .f.
