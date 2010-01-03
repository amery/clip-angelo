static function hp_text(s)
       local i
       s=strtran(s,"&\r","")
       if substr(s,1,8) == '<![CDATA'
		aadd(::tags,s)
		return .t.
       endif
       while .t.
		i=at("&\n",s)
		if i>0
			if i>1
			   aadd(::tags,substr(s,1,i-1) )
			endif
			aadd(::tags,NIL)
			s=substr(s,i+1)
		else
			aadd(::tags,s)
			exit
		endif
       enddo
return .t.
