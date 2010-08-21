static function hp_parse
       local tbeg,tend,s,ch,c:=0,f1:=.f.,sch:='"'
       local len, nPos, c1,c2
       ::error := ""
       while .t.
	     tbeg := at("<",::buffer)
	     if tbeg > 1
		::__addText(substr(::buffer,1,tbeg-1))
		::buffer:=substr(::buffer,tbeg)
		tbeg := 1
	     endif
	     // Check for DOCTYPE or CDATA
	     s :=substr(::buffer,tbeg+1,8)
	     if  s == '!DOCTYPE' .or. s == '![CDATA['
		if s == '!DOCTYPE'
			s := ">"
			nPos := 0
		else
			s := "]>"
			nPos := 1
		endif
		tend:=at(s,::buffer)
		if tend>0
			// Put as text without interpretation
			::__addText(substr(::buffer,1,tend+nPos))
			::buffer:=substr(::buffer,tend+nPos+1)
			loop
		else
			// Missed end: wait for next portion
			exit
		endif
	     endif
	     len := len(::buffer)
#ifdef __1
	     for tend=tbeg to len
		ch=substr(::buffer,tend,1)
		if !f1 .and. (ch=="'" .or. ch=='"')
			f1:=.t.
			sch:=ch
			loop
		endif
		if f1 .and. ch==sch
			f1:=.f.
			loop
		endif
		if !f1 .and. ch=="<"
			c++
		endif
		if !f1 .and. ch==">"
			c--
		endif
		if c==0
			exit
		endif
	     next
	     if tend >= len .and. ch!=">"
		tend := 0
	     endif
#else
	     tend := at(">",::buffer)
	     if tend > len .or. tend <= 0 //.and. ch!=">"
		tend := 0
	     endif
#endif
	     //tend=at(">",::buffer)
	     if tbeg<=0 .or. tend<=0
		exit
	     endif

	     s := substr(::buffer,tbeg+1,tend-tbeg-1)
	     c1 := cscount("'",@s)
	     c2 := cscount('"',@s)
	     if tend >0 .and. ( (c1%2)!=0 .or. (c2%2) != 0)
		::error := [Bad characters in HTML/XML tag.]
	     endif

	     ::__parseTag(@s)
	     ::buffer:=substr(::buffer,tend+1)
       enddo
return .t.
