static function HTTP_checkVhost(self)
	local i,j,x,s,s1,s2,host,sections,found := .f.
	i := ascan(self:sets,{|x|x[1]=="HTTP_HOST"})
	if i<=0
		return
	endif
	host := self:sets[i][2]
	i := atr(":",host)
	if i>0
		host := left(host,i-1)
	endif
	s2 := self:oIni:checkName(host)
	sections := self:oIni:sections()
	for i=1 to len(sections)
		s := sections[i]
		if !(left(s,12) == "HTTP_VIRTUAL")
			loop
		endif
		s1 := substr(s,14)
		if s2 == s1
			found := .t.
			exit
		endif
	next
	if found
		self:fromIni(self:oIni,s)
	endif
return
