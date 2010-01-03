static function sql_parseTo()
	local i,s,w,fname:=""
        ::__sets:=savesets()
        if valtype(::to)!="C"
        	return
        endif
        s:=::to
        ::to:=SQL_TO_NONE
        i:=at(" ",s)
        i:=iif(i<=0,len(s)+1,i)
        w:=upper(substr(s,1,i-1))  // TO word
        s:=alltrim(substr(s,i+1))
        i:=at(" ",s)
        i:=iif(i<=0,len(s)+1,i)
        ::filename:=substr(s,1,i-1)     // if exist - filename
        s:=upper(alltrim(substr(s,i+1))) // if exist - additive
        if s=="ADDITIVE"
        	::additive:=.t.
        endif
        do case
        	case w=="FILE"
                	::to:=SQL_TO_FILE
        	case w=="CURSOR"
                	::to:=SQL_TO_CURSOR
        	case w=="DBF"
                	::to:=SQL_TO_DBF
        	case w=="TABLE"
                	::to:=SQL_TO_TABLE
        	case w=="PRINTER"
                	::to:=SQL_TO_PRINTER
        	case w=="ARRAY"
                	::to:=SQL_TO_ARRAY
        	case w=="MAP"
                	::to:=SQL_TO_MAP
        	case w=="OBJECT"
                	::to:=SQL_TO_OBJECT
        endcase
return
