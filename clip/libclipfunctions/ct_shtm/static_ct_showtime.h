static function ct_showtime(x,y,lSeconds,cColor,l12,lAm)
	local row,col,str,ap
	while !time_close
		str:=time()
		ap:=.f.
		if !( lSeconds==NIL .or. !lSeconds )
			str:=substr(str,1,5)
		endif
		if l12!=NIL .and. l12
			if val(str)>12
				str:=padl(str(val(str)-12,2,0),2,"0")+substr(str,3)
				ap:=.t.
			endif
		endif
		if lAm!=NIL .and. lAm
			str+=iif(ap,"p","a")
		endif
		//taskstop()
		dispbegin()
		row:=row(); col:=col()
		@ x,y say str color cColor
		devpos(row,col)
		//taskstart()
		dispend()
		sleep(1)
	end
return
