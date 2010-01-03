static function sql_parseOrder()
	local a,s,d,x,i,j,m,tmp:={}
        if valtype(::order)!="C" .or. empty(::order)
        	return
        endif
        m:=split(alltrim(::order),",")
        for i=1 to len(m)
               	s:=alltrim(m[i])
               	a:=map()
               	j:=at(" ",s)
               	if j<=0
               		a:direction:=SQL_DIRECTION_ASC
                        a:field:=upper(s)
               	else
                	d:=upper(alltrim(substr(s,1,j-1)))
               		a:direction:=iif(d=="DESC",SQL_DIRECTION_DESC,SQL_DIRECTION_ASC)
                        a:field:=alltrim(upper(substr(s,j+1)))
               	endif
                // numeric value of order field
                d:=val(a:field)
                if d >0
                	if d > len(::fields)
                        	fx_sql_error("Error number column in GROUP BY",__LINE__)
                        endif
                        a:column:=::fields[d]:xname
                else
                	// name table and name field -> table.field
                	j:=at(".",a:field)
               		if j<=0
               			a:file:=::from[1]:alias
                        	a:field:=upper(a:field)
               		else
               			a:file:=alltrim(upper(substr(a:field,1,j-1)))
                        	a:field:=alltrim(upper(substr(a:field,j+1)))
               		endif
                	// search field name in ::fields
                	j:=ascan(::fields,{|x|x:aname==a:file .and. x:xname==a:field})
                        if j<=0
                        	fx_sql_error("Undefined field name in ORDER BY")
                        endif
                        a:column:=::fields[j]:xname
                endif
                aadd(tmp,a)
        next
        /*
        // generate codeblock for asort
	s:="{|x1,x2| "
	for i in tmp
        	s+="x1:"+i:column
                s+=iif(i:direction==SQL_DIRECTION_ASC,"<",">")
                s+="x2:"+i:column
                s+=" .or. "
        next
        s:=substr(s,1,len(s)-6)+"}"
        ::order:=&s
        */
        ::order:=tmp
#ifdef DEBUG_ORDER
        ? "order tmp=",tmp
        //? "codeblock=",s
#endif
return
