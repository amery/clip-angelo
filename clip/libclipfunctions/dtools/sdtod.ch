********************************************************************
* test for sdtod()
******
*local i,m,d1,d2
*m:={"01.02.03","02/03/04","20050607","3k05","2�04","1q03","3m05","2�04","3n05","2�04","11w03"}
*? week(date())
*for i=1 to len(m)
*	? sdtod(m[i],@d1,@D2),d1,d2
*next
*?
********************************************************************
function sdtod(s,d1,d2)
	local i, forms:={"dd.mm.yy","yy.mm.dd","mm.dd.yy"}
	local s1,s2,s3
	d1:=stod("        ")
	d2:=stod("        ")
	s:=strtran(s," ","")
	s:=strtran(s,"/",".")
	s:=strtran(s,"-",".")
	d1 := stod(s)
	if !empty(d1) .and. d1==stod(dtos(d1))
		d2:=d1
		return .t.
	endif
	for i=1 to len(forms)
		d1 := ctod(s,forms[i])
		if !empty(d1) .and. d1==ctod(dtoc(d1,forms[i]),forms[i])
			d2:=d1
			exit
		endif
	next
	if !empty(d1)
		return .t.
	endif
	if !isdigit(s)
		return .f.
	endif
	for i=1 to len(s)
		if !isdigit(substr(s,i,1))
			exit
		endif
	next
	s1 := padl(substr(s,1,i-1),2,"0")
	s3 := substr(s,i)
	for i=1 to len(s3)
		if isdigit(substr(s3,i,1))
			exit
		endif
	next
	s2 := upper(substr(s3,1,i-1))
	s3 := substr(s3,i+1)
	if s2 $ [KQ�]     /* quartel */
		s1 := val(s1)
		if s1<1 .or. s1>4
			return .f.
		endif
		s1 := (s1-1)*3+1
		d1 := ctod(s3+"."+str(s1,2,0)+".01","yy.mm.dd")
		d2 := ctod(s3+"."+str(s1+2,2,0)+".01","yy.mm.dd")
		d2 := eom(d2)
		return .t.
	elseif s2 $ [M�]  /* month   */
		i := val(s1)
		if i<1 .or. i>12
			return .f.
		endif
		d1 := ctod(s3+"."+s1+".01","yy.mm.dd")
		d2 := eom(d1)
		return .t.
	elseif s2 $ [WN�] /* week    */
		s1 := val(s1)
		if s1<1 .or. s1>60
			return .f.
		endif
		d1 := ctod(s3+".01.01","yy.mm.dd")
		d1 += (s1-2)*7
		while week(d1)<s1
			d1++
		end
		d1++
		d2:=d1+6
		return .t.
	else
		return .f.
	endif
return .f.
