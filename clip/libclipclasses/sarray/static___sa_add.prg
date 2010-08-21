static function __SA_Add(key,value)
	local lExact:=set(_SET_EXACT,.t.)
	local nLen:=len(::items)
	local nPos,bPos:=1, ePos:=nLen
	::error:=""
	if key == NIL .or. value == NIL
		::error:=[Unknown values for key or data]
		set(_SET_EXACT,lExact)
		return .f.
	endif
	if nLen==0
		aadd(::items,{key,value})
	elseif key<::items[1][1]
		aadd(::items,"")
		ains(::items,1,{key,value})
	elseif key>=::items[nLen][1]
		aadd(::items,{key,value})
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
				for nPos=ePos to bPos step -1
				    if key>=::items[nPos][1]
					nPos++
					exit
				    endif
				next
				exit
			endif
		enddo
		aadd(::items,"")
		ains(::items,nPos,{key,value})
	endif
	set(_SET_EXACT,lExact)
return .t.
