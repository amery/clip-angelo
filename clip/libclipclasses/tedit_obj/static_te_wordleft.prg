static function te_wordLeft()
   local s,str, p:=::pos, fl:=.f., pl:=.f.
   if ::line>::lines
	return
   endif
   str:=::edBuffer[::line]
   while p>0
	s=substr(str,p,1)
	if s!=" " .and. (isalpha(s) .or. isdigit(s) .or. s=="_")
	   pl:=.t.
	else
	   if fl .and. pl
	      exit
	   endif
	   fl:=.t.
	endif
	p--
   enddo
   ::pos:=p+1
   ::colWin:=max((::nRight-::nLeft)*1/6,::colWin)
   ::refresh()
RETURN
