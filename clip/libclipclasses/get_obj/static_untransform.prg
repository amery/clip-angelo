static function unTransform()
   local s ,i,j, s1,s2,s3,s4
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"untransform",::name)
#endif
    // buffer ���� ��������� �� �������� ��������
    if "R" $ ::__flags .or. ::type!="C"
       s=""
       for i=1 to len(::__posArr)
	  s+=substr(::buffer,::__posArr[i],1)
       next
    else
       s:=::buffer
    endif
    do case
       case ::type=="C"
       case ::type=="T"
	    s:=CTOT(::buffer)
       case ::type=="D"
	    s1:=""
	    s4:=::__dateFormat
	    j:=1
	    for i=1 to len(s4)
		s2:=substr(s4,i,1)
		if s2 $ DATE_DELIMITER_CHARS
		    s1+=s2
		else
		    s1+=substr(s,j,1)
		    j++
		endif
	    next
	    s:=CTOD(s1,::__dateFormat)
       case ::type=="L"
	    s:=iif(left(s,1) $ ("Y"+LANG_YES_CHAR),.t.,.f.)
       case ::type=="M"
       case ::type=="N"
	    if ::decpos==0
		s:=val(s,len(s),0)
	    else
		j:=len(strtran(::__dataSay," ",""))
		//s:=substr(s,1,::decpos-j)+"."+substr(s,::decpos-j+1)
		s:=left(s,len(s)-::__dec)+"."+right(s,::__dec)
		s:=val(s,::__len,::__dec)
	    endif
       otherwise
	    s:=::_original
    endcase
return s
