static func lb_findText(mtext,pos,lcase,lexact)
	local bl,ret
	if valtype(mtext)!="C"
	   return ::value
	endif
	pos:=iif(pos==NIL,1,pos)
	lcase:=iif(lcase==NIL,.t.,lcase)
	lexact:=iif(lexact==NIL,set(_SET_EXACT),lexact)
	if lcase
	   mtext:=lower(mtext)
	   if lexact
		bl:={ |x| lower(x)==mtext }
	   else
		bl:={ |x| lower(x)=mtext }
	   endif
	else
	   if lexact
		bl:={|x| x==mtext }
	   else
		bl:={|x| x=mtext }
	   endif
	endif
	ret:=ascan(::__mText,bl,pos,len(::__mText))
return ret
