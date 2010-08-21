static function hp_parseTag(str)
	local i,i1,i2,i3,t,fname:="",fdata:=""
	local f1:=.t.,f2:=.f.,f3:=.f.,ch,sch:='"'
	str:=alltrim(str)
	str:=strtran(str,"&\t"," ")
	str:=strtran(str,"&\r","")
	//str:=strtran(str,"&\n"," ")
	if left(str,1)=="!"
		for i=2 to len(str)
			if !(substr(str,i,1)=="-")
				exit
			endif
		next
		str:=substr(str,i)
		for i=len(str) to 1 step -1
			if !(substr(str,i,1)=="-")
				exit
			endif
		next
		str:=substr(str,1,i)
		t=html_tagNew("!")
		t:addField("DATA",str)
		return .t.
	endif
	i:=1
	ch := substr(str,i,1)
	while (isalpha(ch) .or. ch $ "?/_1234567890") .and. !(ch $ " &\t&\n")
		i++
		ch := substr(str,i,1)
	enddo
	if i==0
		i:=len(str)+1
	endif
	t=html_tagNew(substr(str,1,i-1))
	if substr(str,len(str),1) == "/"
		t:closed := .t.
	endif
	str=alltrim(substr(str,i+1))
	while !empty(str)
		ch:=left(str,1)
		str:=substr(str,2)
		if f1 .and. ch $ " &\n"
			loop
		endif
		if ch="=" .and. !f3
			f1:=.f.
			f2:=.t.
			while .t.
				ch:=left(str,1)
				if ch $ " &\n"
					str:=substr(str,2)
				else
					exit
				endif
			enddo
			loop
		endif
		if (ch=='"' .or. ch=="'") .and. !f3
			f3:=.t.
			sch:=ch
			loop
		endif
		if ch==sch .and. f3
			f3:=.f.
			loop
		endif
		if ch=="/" .and. !f3
			loop
		endif
		if (ch $ " &\n" .or. ch=="/") .and. !f3
			f2:=.f.
			f1:=.t.
			t:addField(fname,fdata)
			fname:=""
			fdata:=""
			if ch=="/"
				exit
			else
				loop
			endif
		endif
		if f1
		   fname+=ch
		endif
		if f2
		   fdata+=ch
		endif
	enddo
	if !empty(fname) .or. !empty(fdata)
		t:addField(fname,fdata)
	endif
	aadd(::tags,t)
return .t.

