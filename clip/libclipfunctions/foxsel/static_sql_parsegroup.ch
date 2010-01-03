static function sql_parseGroup()
	local a,s,d,x,i,j,m,tmp:={}
        if valtype(::group)!="C" .or. empty(::group)
        	return
        endif
        m:=split(alltrim(::group),",")
        if len(m) > 0
        	::groupBlocks:={}
        endif

        for i=1 to len(m)
               	s:=alltrim(m[i])
               	a:=map()
               	a:hDb:=NIL
               	a:direction:=SQL_DIRECTION_ASC
               	j:=at(".",s)
               	if j<=0
               		a:file:=::from[1]:alias
                       	a:field:=upper(s)
               	else
               		a:file:=alltrim(upper(substr(s,1,j-1)))
                       	a:field:=alltrim(upper(substr(s,j+1)))
               	endif
                d:=val(a:field)
                if d >0
                	if d > len(::fields)
                        	fx_sql_error("Error number column in GROUP BY",__LINE__)
                        endif
                        a:column := ::fields[d]:xname
                        a:file   := ::fields[d]:aname
                        a:field  := ::fields[d]:fname
                	j:=ascan(::from,{|x|x:alias==a:file})
                	if j<=0
                		fx_sql_error("Undefine table reference in GROUP BY",__LINE__)
                	endif
                        a:hDB	 := ::from[j]:hDB
                else
                	j:=ascan(::from,{|x|x:alias==a:file})
                	if j<=0
                		fx_sql_error("Undefine table reference in GROUP BY",__LINE__)
                	endif
                	d:=::from[j]
                	j:=ascan(d:dbStruct,{|x|x[1]==a:field})
                	if j<=0
	                	fx_sql_error("Undefine field name in GROUP BY",__LINE__)
        	        endif
                	j:=ascan(::fields,{|x|x:aname==a:file .and. x:fname==a:field})
                	if j>0
                		a:column := ::fields[j]:xname
                	endif
                	a:hDb:=d:hdb
                endif
                if i==1 .and. empty(::order)
                	::order:={}
                        aadd(::order,a)
                endif
                aadd(tmp,a)
        next
#ifdef DEBUG_GROUP
        ? "group tmp=",tmp
#endif
	for i in tmp
        	s:="{|x|rddGetValue("+alltrim(str(i:hDb))+",'"+i:field+"')}"
                aadd(::groupBlocks,&s)
        next
return
