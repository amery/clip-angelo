static function __toString()
  local ret,i,s,ch,flag
  ret:=::buffer
  if "B" $ ::__flags
	ret:=padr(alltrim(ret),len(ret))
  endif
  if "S" $ ::__flags
	ret:=substr(ret,1,::__winlen)
  endif
  if "(" $ ::__flags .and. ::type=="N" .and. ::_original<0
	ret:="("+ret+")"
  endif
  if ::type=="N"
	if "," $ ::__dataSay
		s:=ret; ret:=""; flag:=.f.
		for i=1 to len(s)
			ch:=substr(s,i,1)
			if isdigit(ch)
				flag:=.t.
			endif
			if flag
				ret+=ch
				loop
			endif
			ret+=iif(ch==","," ",ch)
		next
	endif
	if "E" $ ::__flags
		s:=ret; ret:=""; flag:=.f.
		for i=1 to len(s)
			ch:=substr(s,i,1)
			if isdigit(ch)
				flag:=.t.
			endif
			if flag
				ret+=ch
				loop
			endif
			ret+=iif(ch=="."," ",ch)
		next
	endif
	if ::_original==0 .and. "Z" $ ::__flags
		ret:=space(len(ret))
	endif
	ret := strtran(ret,"-,"," -")
	ret := strtran(ret,"- "," -")
  endif
return ret
