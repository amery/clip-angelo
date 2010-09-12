static function te_wordRight()
   local s,str, p:=::pos, fl:=.f.
   if ::line>::lines
	return
   endif
   str:=::edBuffer[::line]
   while p<len(str)
	s=substr(str,p,1)
	if s!=" " .and. (isalpha(s) .or. isdigit(s) .or. s=="_")
	   if fl
	      exit
	   endif
	else
	   fl:=.t.
	endif
	p++
   enddo
   ::pos:=p
   ::colWin:=(::nRight-::nLeft)*5/6
   ::refresh()
RETURN
