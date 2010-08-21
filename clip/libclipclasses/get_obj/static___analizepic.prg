static function __analizePic()
  local s1:="",s2:="",s3:="",s4,i,j,sf:=.f.,df
  //local len,p:="ANX9#LY!$* "
  local len,p:="ANX9#LY!$*"
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"__analizePic",::name)
#endif
  ::__winlen:=0
  ::__flags:=""
  ::__posArr:={}
  ::__format:=""
  ::__dataSay:=""
  //if ::type=="N"
     ::__len:=0
     ::__dec:=0
     ::decpos:=0
  /*
  else
     ::__len:=NIL
     ::__dec:=NIL
     ::decpos:=NIL
  endif
  */
  if !empty(::picture)
	  // ������ ������ � PICTURE-������
	s1:=::picture
	if substr(s1,1,1)=="@"
		i:=at(" ",s1)
		i:=iif(i==0,len(s1)+1,i)
		s4=substr(s1,i+1)
		s1=upper(substr(s1,2,i-2))
		for i=1 to len(s1)
			s2=substr(s1,i,1)
			if sf
				if isdigit(s2)
					s3+=s2
					loop
				else
					sf := .f.
				endif
			endif
			if s2=="@"
				loop
			endif
			sf:=(s2=="S")
			::__flags+=s2
			if sf; s3 := ""; endif
		next
	else
		s4:=::picture
	endif
	if empty(s3)
		::__winlen:=len(s4)
	else
		::__winLen:=val(s3)
		::__maxLen:=::__winLen
	endif
  endif
  if ::type $ "DT"
     if "E"$::__flags
	 if __setCentury()
	     s4:=DATE_BRITISH_FORMAT_CENTURY
	 else
	     s4:=DATE_BRITISH_FORMAT
	 endif
     else
	 s4:=upper(set(_SET_DATEFORMAT))
     endif
     ::__dateFormat:=s4
     df:=""
     if ::type=="T"
	s4 +=" hh:mm"
	if set(_SET_SECONDS)
		s4 +=":ss"
	endif
	s4 +="A"
     endif
     for i=1 to len(s4)
		s2=substr(s4,i,1)
		if s2 $ DATE_DELIMITER_CHARS
			::__format+=" "
			::__dataSay+=s2
		else
			::__format+=iif(s2=="a","X","9")
			::__dataSay+=" "
			aadd(::__posArr,i)
			if s2 $ "DMY"
				df+=s2
			endif
		endif
     next
     if ::type=="T"
	::__original:=TTOC(::_original,df)
	if "M" $ upper(::__original)
		::__format +=" "
		::__dataSay+="M"
	endif
	::__original:=strtran(::__original,":","")
	//::__original:=strtran(::__original," ","")
	::__original := left(::__original,len(df))+substr(::__original,len(df)+2)
     else
	::__original:=DTOC(::_original,df)
     endif
     ::__winlen:=len(::__format)
     ::__maxLen:=::__winLen
     //::__flags+="K"
     return NIL
  endif
  if empty(s4) //empty(::picture)
     // ������� ������ �� ���������
	do case
	case ::type=="C"
		  ::__len:=len(::__original)
		  ::__format:=replicate("X",::__len)
		  ::__dataSay:=space(::__len)
		  for i=1 to ::__len
		      aadd(::__posArr,i)
		  next
	case ::type=="T"
		  ::__format:=""
		  ::__dataSay:=""
	case ::type=="D"
		  ::__format:=""
		  ::__dataSay:=""
	case ::type=="L"
		  ::__original:= left(::__original,1)
		  ::__dataSay:=::__original
		  ::__format:="Y"+space(len(::__original)-1)
		  aadd(::__posArr,1)
	case ::type=="N"
		::__len:=len(::__original)
		::decpos:=::__len+1
		for i=1 to ::__len
			s2=substr(::__original,i,1)
			if s2=="."
				s1+="."
				::decpos:=i
				::__format+=" "
				::__dataSay+="."
			else
				::__format+="9"
				::__dataSay+=" "
				aadd(::__posArr,i)
			endif
		next
		::__dec:=::__len-::decpos
		::__dec:=max(::__dec,0)
		::__flags+="K"
	case ::type=="M"
		::__flags+="R"
		::__winlen:=maxcol()-::col-1
		::__format:=replicate("X",GETOBJ_MAX_MEMO_LEN)
		::__maxLen:=GETOBJ_MAX_MEMO_LEN
		::__dataSay:=space(GETOBJ_MAX_MEMO_LEN)
		  for i=1 to GETOBJ_MAX_MEMO_LEN
		      aadd(::__posArr,i)
		  next
	case ::type=="A"
		::__dataSay:=::__original
		::__format:=space(len(::__original))
	case ::type=="B"
		::__dataSay:=::__original
		::__format:=space(len(::__original))
	case ::type=="O"
		::__dataSay:=::__original
		::__format:=space(len(::__original))
	case ::type=="P"
		::__dataSay:=::__original
		::__format:=space(len(::__original))
	otherwise
		::__dataSay:=::__original
		::__format:=space(len(::__original))
	endcase
	if !("S" $ ::__flags) .or. ::__winlen==0
	   ::__winlen:=min(len(::__format),iif(::__fullScreen,maxcol(),999)-::col)
	endif

  else
	//  ������ ������� ������ ������
	::__format:=""
	::__dataSay:=""
	for i=1 to len(s4)
		s2=substr(s4,i,1)
		if s2=="." //.and. ::type=="N"
			::decpos:=i
			::__len++
		endif
		if upper(s2) $ iif(::type=="N","9#X",p)
			::__format+=upper(s2)
			::__dataSay+=" "
			aadd(::__posArr,i)
			//if ::type=="N"
			::__len++
			if ::decpos>0
				::__dec++
			endif
			//endif
		else
			::__format+=" "
			::__dataSay+=s2
		endif
	next
	if ::type == "N" .and. ::__len == ::decpos .and. ::__dec==0
		::__len--
	endif
	if ::type=="N"
		::__flags+="K"
	endif
	if ::type=="N" .and. "E" $ ::__flags
		for i=1 to len(::__dataSay)
			s2:=substr(::__dataSay,i,1)
			if !(s2 $ ",.")
				loop
			endif
			::__dataSay:=substr(::__dataSay,1,i-1)+;
				iif(s2==".",",",".")+substr(::__dataSay,i+1)
		next
	endif
	if ::__winlen==0
		::__winlen:=min(len(::__format),maxcol()-::col)
	endif
	if ::type == "L"
		  if "L" $ ::picture
			::__original:= left(::__original,1)
		  endif
		  ::__dataSay:=::__original
		  ::__format:="Y"+space(len(::__original)-1)
		  ::__posArr := {1}
	endif
  endif
  if len(::__posArr)==0 .and. ::expand
	aadd(::__posArr,1)
	::pos:=1
	::__format+="X"
  endif
return NIL
