static function __SA_Seek(key,lSoft)
	local lExact:=set(_SET_EXACT,.t.)
	local nLen:=len(::items),kLen:=len(key), ret:=.f.
	local nPos:=-1,bPos:=1, ePos:=nLen
	lSoft:=iif(lSoft==NIL,.f.,lSoft)
	::found := .f.
	::error:=""
	if key == NIL
		::error:=[Unknown values for key]
		set(_SET_EXACT,lExact)
		return -1
	endif
	if nLen==0
	elseif key <  ::items[1][1]
		nPos:=iif(lSoft,1,-1)
	elseif key > ::items[nLen][1]
	else
		nPos:=int(nLen/2)
		do while .t.
			if ePos-bpos < 5
				if !lSoft .or. valtype(key) != "C"
					for nPos=bPos to ePos
						if key==::items[nPos][1]
							exit
						endif
					next
				else
					for nPos=bPos to ePos
						if key<=left(::items[nPos][1],kLen)
							exit
						endif
					next
				endif
				exit
			endif

			if key>=::items[nPos][1]
				bPos=nPos-1
				nPos:=bPos+int((ePos-bPos)/2)
//				bPos:=iif(bPos<1,1,bPos)
			else
				ePos=nPos+1
				nPos:=bPos+int((ePos-bPos)/2)
			endif
		enddo
	endif
	set(_SET_EXACT,lExact)
	if nPos<1 .or. nPos>nLen
		return nLen+1
	endif
	if !lSoft .and. key!=::items[nPos][1]
		return nLen+1
	endif
	::found:= ( key==::items[nPos][1] )
return nPos
