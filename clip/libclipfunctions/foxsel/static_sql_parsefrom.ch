static function sql_parseFrom()
	local a,s,i,j,m
        if valtype(::from)!="C"
        	return
        endif
        if empty(::from)
        	fx_sql_error("Sql parse FROM description",__LINE__)
        endif
        s:=alltrim(::from); ::from:={}
        m:=split(s,",")
        for i=1 to len(m)
               	s:=alltrim(m[i])
               	a:=map()
               	a:hDb:=NIL
               	a:order:=i
               	j:=at(" ",s)
               	if j<=0
               		a:file:=s
                        a:alias:=upper(s)
               	else
               		a:file:=substr(s,1,j-1)
                        a:alias:=upper(substr(s,j+1))
               	endif
                j := atr("/",a:alias)
                if j<=0
                	j := atr("\",a:alias)
                endif
                if j>0
                	a:alias := substr(a:alias,j+1)
                endif
               	a:order:=len(::from)+1
               	aadd(::from,a)
        	if ::subQuery  // only first field
                	exit
                endif
        next
return
