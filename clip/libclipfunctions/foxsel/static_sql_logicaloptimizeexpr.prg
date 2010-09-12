static function sql_logicalOptimizeExpr(data)
	local i,j,k,x, tags:={},ltags:={}
        local word,aname,fname
        local beg,end,lexit:=.f.
        local lVal,rVal,lOper,rOper
        local onlyAnd,onlyOr
#ifdef DEBUG_OPTIMIZE_WHERE
        ? "opt data=",data
#endif
        for i=1 to len(data)
        	word:=data[i]
                // compare operation
        	if valtype(word)=="N" .and. word==SQL_OPER_BETWEEN
                	if (len(data)-i)<3
                        	fx_sql_error("Error in BETWEEN",__LINE__)
                        endif
                	if valtype(data[i+2])!="N" .or. ;
                	   data[i+2] != SQL_OPER_AND
                        	fx_sql_error("Error in BETWEEN",__LINE__)
                        endif
                        lVal:=data[i+1]
                        lVal:=&lVal
                        rVal:=data[i+3]
                        rVal:=&rVal
                        if lVal == rVal
                        	lVal:=rVal:=data[i+1]
                        else
                        	if lVal>rVal
                        		rVal:=data[i+1]
                        		lVal:=data[i+3]
                                else
                        		lVal:=data[i+1]
                        		rVal:=data[i+3]
                                endif
                        endif
                        if lVal==rVal
                     		x:=map()
                        	x:type=SQL_TAG_OPER
                        	x:oper:=SQL_OPER_EXACTEQUAL
                        	x:not:=.f.
                        	aadd(tags,x)
                                *
                     		x:=map()
                        	x:type=SQL_TAG_CONST
                                x:value:=lVal
                        	x:not:=.f.
                        	aadd(tags,x)
                        else
                        	k:=atail(tags)
                                asize(tags,len(tags)-1)
                     		x:=map()
                        	x:type=SQL_TAG_GROUP
                        	x:oper:=SQL_OPER_GROUPOPEN
                        	x:not:=.f.
                        	aadd(tags,x)
                                *
                        	aadd(tags,k)
                                *
                     		x:=map()
                        	x:type=SQL_TAG_OPER
                        	x:oper:=SQL_OPER_GREATEQUAL
                        	x:not:=.f.
                        	aadd(tags,x)
                                *
                     		x:=map()
                        	x:type=SQL_TAG_CONST
                                x:value:=lVal
                        	x:not:=.f.
                        	aadd(tags,x)
                                *
                     		x:=map()
                        	x:type=SQL_TAG_LOGICAL
                                x:oper:=SQL_OPER_AND
                        	x:not:=.f.
                        	aadd(tags,x)
                                *
                        	aadd(tags,k)
                                *
                     		x:=map()
                        	x:type=SQL_TAG_OPER
                        	x:oper:=SQL_OPER_LESSEQUAL
                        	x:not:=.f.
                        	aadd(tags,x)
                                *
                     		x:=map()
                        	x:type=SQL_TAG_CONST
                                x:value:=rVal
                        	x:not:=.f.
                        	aadd(tags,x)
                                *
                     		x:=map()
                        	x:type=SQL_TAG_GROUP
                        	x:oper:=SQL_OPER_GROUPCLOSE
                        	x:not:=.f.
                        	aadd(tags,x)
                        endif
                        i+=3
                	loop
                endif
        	if valtype(word)=="N" .and. word<=SQL_OPER_IN
                     	x:=map()
                        x:type=SQL_TAG_OPER
                        x:oper:=word
                        x:not:=.f.
                        aadd(tags,x)
                        loop
                endif
                // subquery
        	if valtype(word)=="N" .and.;
		   word >= SQL_OPER_EXIST .and. word <= SQL_OPER_SOME
                   	if i>=len(data)
                        	fx_sql_error("bad expression in WHERE:",__LINE__)
                        endif
                     	x:=map()
                        x:type=SQL_TAG_OPER
                        x:oper:=word
                        x:not:=.f.
                        aadd(tags,x)
                     	x:=map()
                        x:type=SQL_TAG_SUBQUERY
                        x:value:=data[i+1]
                        x:not:=.f.
                        aadd(tags,x)
                        i++  // next tag
                        loop
                endif
                // logical operation
        	if valtype(word)=="N" .and. ;
                   (word==SQL_OPER_GROUPOPEN .or. word==SQL_OPER_GROUPCLOSE)
                     	x:=map()
                        x:type=SQL_TAG_GROUP
                        x:oper:=word
                        x:not:=.f.
                        aadd(tags,x)
                        loop
                endif
        	if valtype(word)=="N"
                     	x:=map()
                        x:type=SQL_TAG_LOGICAL
                        x:oper:=word
                        x:not:=.f.
                        aadd(tags,x)
                        loop
                endif
                // call function
        	if valtype(word)=="C" .and. "(" $ word
                	::functions:=.t.
                     	x:=map()
                        x:type=SQL_TAG_FUNC
                        x:func:=word
                        x:not:=.f.
                        aadd(tags,x)
                        loop
                endif
        		// data for IN,ANY,ALL,SOME
        	if valtype(word)=="C"
                	if i>1 .and. valtype(data[i-1])=="N"
                		x:=data[i-1]
                                if x>=SQL_OPER_IN .and. x<=SQL_OPER_SOME
                                        if x==SQL_OPER_IN .and. "select" $ lower(word)
                                        	data[i-1]:=SQL_OPER_ANY
                                        endif
                     			x:=map()
                        		x:type=SQL_TAG_CONST
                        		x:value:=word
                        		x:not:=.f.
                        		aadd(tags,x)
                                        // it`s not IN - it`s ANY
                        		loop
                                endif
                        endif
                endif
                	// field & table names
        	if valtype(word)=="C" .and. "." $ word
                	// check for table.field
                        aname:=""; fname:=""
                	j:=at(".",word)
                        aname:=alltrim(upper(substr(word,1,j-1)))
                        fname:=alltrim(upper(substr(word,j+1)))
                        // check table in FROM
                        j:=ascan(::from,{|x|x:alias==aname} )
                        if j<=0
                        	fx_sql_error("bad table reference in WHERE:"+word,__LINE__)
                        endif
                        // check field name in table
                        k:=ascan(::from[j]:dbStruct,{|x|x[1]==fname})
                        if k<=0
                        	fx_sql_error("bad table or field reference in WHERE:"+word,__LINE__)
                        endif
                     	x:=map()
                        x:type=SQL_TAG_FIELD
                        x:table:=aname
                        x:field:=fname
                        x:hDB:=::from[j]:hDB
                        x:not:=.f.
                        x:xname := NIL
                        for k in ::fields
                               	if k:aname==aname .and. k:fname==fname
                                       	x:xname := k:xname
                                endif
                        next
                        aadd(tags,x)
                        loop
                endif
                // field name one table name
        	if valtype(word)=="C"
                	fname:=alltrim(upper(word))
                        aname:=::from[1]:alias
                        k:=ascan(::from[1]:dbStruct,{|x|x[1]==fname})
			if k>0  // field name one table name
                     		x:=map()
                        	x:type=SQL_TAG_FIELD
                        	x:table:=aname
                        	x:field:=fname
                                x:hDB:=::from[1]:hDB
                                x:not:=.f.
                                x:xname := NIL
                                for k in ::fields
                                	if k:aname==aname .and. k:fname==fname
                                        	x:xname := k:xname
                                        endif
                                next
                        	aadd(tags,x)
                                loop
                        endif
                        // constant
                     	x:=map()
                        x:type=SQL_TAG_CONST
                        x:value:=word
                        x:not:=.f.
                        aadd(tags,x)
                endif
        next
#ifdef DEBUG_OPTIMIZE_WHERE
        ? "tags="
        for i in tags
        	? i
        next
#endif
	// converting NOT and compare operation
	ltags:={}
	for i=1 to len(tags)
        	k:=tags[i]
        	if k:type!=SQL_TAG_OPER
                	aadd(ltags,k)
                	loop
                endif
                lVal:=rVal:=NIL
                if i>1
                	lVal:=tags[i-1]
                else
        		lVal:=map(); lVal:type:=SQL_TAG_CONST; lVal:value:=".t."
                endif
                if i<len(tags)
                	rVal:=tags[i+1]
                else
        		rVal:=map(); rVal:type:=SQL_TAG_CONST; rVal:value:=".t."
                endif
                if lVal:type>=SQL_TAG_FUNC
                	asize(lTags,len(lTags)-1)
                else
        		lVal:=map(); lVal:type:=SQL_TAG_CONST; lVal:value:=".t."
                endif
                if rVal:type>=SQL_TAG_FUNC
                	i++
                else
        		rVal:=map(); rVal:type:=SQL_TAG_CONST; rVal:value:=".t."
                endif
                x:=map()
                x:type:=SQL_TAG_COMPARE
                x:lvalue:=lVal
                x:rValue:=rVal
                x:oper:=k:oper
                x:not:=.f.
                aadd(lTags,x)
        next
#ifdef DEBUG_OPTIMIZE_WHERE
        ? "ltags="
        for i in ltags
        	? i
        next
#endif
        // converting logical bulev operation and delete GROUP operation
        lexit:=.f.
        while !lExit
        	lexit:=.t.
        	beg:=end:=j:=x:=0
                onlyAnd:=onlyOr:=.t.
        	for i=1 to len(ltags)
                	k:=lTags[i]
                        if k:type==SQL_TAG_COMPARE
                                if lVal:type==SQL_TAG_CONST
                        		lVal := k:rValue
                        		rVal := k:lValue
                        		k:lValue:=lVal
                        		k:rValue:=rVal
                                        k:not:= !k:not
                                endif
                        endif
                        if k:type==SQL_TAG_LOGICAL .and. k:oper==SQL_OPER_NOT
                        	if i<len(lTags)
                                	if lTags[i+1]:type!=SQL_TAG_GROUP
                                		lTags[i+1]:not:=!(lTags[i+1]:not)
                                        	adel(lTags,i)
                                        	asize(lTags,len(lTags)-1)
                                        	i--
                                        	lExit:=.f.
                                        	loop
                                        endif
                                endif
                        endif
                        if k:type!=SQL_TAG_GROUP
                        	if j!=1
                                	loop
                                endif
                                if k:type!=SQL_TAG_LOGICAL
                                	loop
                                endif
                                if k:oper!=SQL_OPER_NOT
                        		x++
                                endif
                                if k:oper==SQL_OPER_AND
                                	onlyOr:=.f.
                                endif
                                if k:oper==SQL_OPER_OR
                                	onlyAnd:=.f.
                                endif
                        endif
                        if k:oper==SQL_OPER_GROUPCLOSE
                        	j--
                                if j!=0
					loop
                                endif
                                end=i
                                if (end-beg)==2 .or. x<=1
                                	adel(lTags,end)
                                	adel(lTags,beg)
                                        asize(lTags,len(lTags)-2)
                                        i-=2
                                        lExit:=.f.
                                	beg:=end:=j:=x:=0
                                	onlyAnd:=onlyOr:=.t.
                                        loop
                                endif
                                if !onlyAnd .and. !onlyOr
                                	beg:=end:=j:=x:=0
                                	loop
                                endif
                                lOper:=rOper:=iif(onlyAnd,SQL_OPER_OR,SQL_OPER_AND)
                                if beg>1
                                	lOper:=lTags[beg-1]
                                        if lOper:type==SQL_TAG_LOGICAL
                                        	lOper:=lOper:oper
                                        else
                                        	lOper:=SQL_OPER_AND
                                        endif
                                endif
                                if end<len(ltags)
                                	rOper:=lTags[end+1]
                                        if rOper:type==SQL_TAG_LOGICAL
                                        	rOper:=rOper:oper
                                        else
                                        	rOper:=SQL_OPER_AND
                                        endif
                                endif
                                if onlyAnd .and. ;
                                  (lOper==SQL_OPER_AND .or. rOper==SQL_OPER_AND)
                                	adel(lTags,end)
                                	adel(lTags,beg)
                                        asize(lTags,len(lTags)-2)
                                        i-=2
                                        lExit:=.f.
                                endif
                                if onlyOR .and. ;
                                  (lOper==SQL_OPER_OR .or. rOper==SQL_OPER_OR)
                                	adel(lTags,end)
                                	adel(lTags,beg)
                                        asize(lTags,len(lTags)-2)
                                        i-=2
                                        lExit:=.f.
                                endif
                                onlyAnd:=onlyOr:=.t.
                                beg:=end:=j:=x:=0
                                loop
                        endif
                        if k:oper==SQL_OPER_GROUPOPEN
                        	if beg==0
                        		beg:=i
                                endif
                                j++
                        endif
        	next
        enddo
        *
#ifdef DEBUG_OPTIMIZE_WHERE
        ? "ltags1="
        for i in ltags
        	? i
        next
#endif
return ltags
