static function HTTP_parseHeader(self)
	local lExit := .f.,cLine:=self:cLine
	local i,j,s,s1,s2,m:={}
	while !lExit
		i := at(chr(13)+chr(10),cLine)
		if i<=0
			i := len(cLine)+2
			lExit := .t.
		endif
		s := left(cLine,i-1)
		cLine := substr(cLine,i+2)
		j := at(":",s)
		if j<=0
			loop
		endif
		s1 := upper(left(s,j-1))
		s1 := "HTTP_"+strtran(s1,"-","_")
		s2 := alltrim(substr(s,j+1))
		aadd(self:sets,{s1,s2})
	enddo
	self:cLine := ""
return
