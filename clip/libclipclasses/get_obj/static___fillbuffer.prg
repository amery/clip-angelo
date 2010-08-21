static function __fillBuffer()
     local i,j:=0,k:=1,s1,s2,s3,len
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"__fillbuffer",::name)
#endif
     ::__first:=.f.
     ::buffer:=""
     if ::type=="L"
	::buffer := ::__dataSay
     else
	if ::type=="C"
		//::__original := padr(::__original,::__len)
		::__original := left(::__original,max(::__len,len(::__format)))
	endif
	len := len(::__posArr)
	if ::type=="C" .and. empty(::__dataSay) .and. !("!" $ ::__format)
		//::buffer := ::__original
		::buffer := padr(::__original,len(::__dataSay))
	else
		i:=1
		while i<=len
			k:=::__posArr[i]
			s1:=substr(::__dataSay,j+1,k-j-1)
			s2:=substr(::__original,i,1)
			s3:=substr(::__format,i,1)
			::buffer+=s1
			if "R" $ ::__flags
				//if s1==s2
				//	::__original:=left(::__original,i-1)+substr(::__original,i+1)
				//	s2:=substr(::__original,i,1)
				//endif
			else
				if ::type $ "CM"
					s2:=substr(::__original,k,1)
				endif
			endif
			s2:=iif(s3=="!",upper(s2),s2)
			::buffer+=s2
			j:=k
			i++
		enddo
		::buffer+=substr(::__dataSay,j+1,len(::__dataSay))
	endif
     endif
     if "!" $ ::__flags
	::buffer := upper(::buffer)
     endif
     if ::type=="N"
	//::buffer := strtran(::buffer,"-,"," -")
	//::buffer := strtran(::buffer,"- "," -")
     endif
return NIL
