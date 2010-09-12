static function sql_makeFilters(data,onlyMain)
	local i,j,s,k,x,y,ss,ss1,ss2,fname
        local ret:={}, lValGet
        local onlyAnd:=.t.,onlyOR:=.t., functions:=.f.
        local allInOneTable:=0
        local lVal,rVal
        *
	// make filter expression for tables
        j:=0
        for k in data
                if k:type==SQL_TAG_FUNC .and. j==0
                	functions:=.t.
                endif
                if k:type==SQL_TAG_COMPARE .and. j==0
                	if k:lValue:type==SQL_TAG_FUNC
                		functions:=.t.
                        endif
                	if k:rValue:type==SQL_TAG_FUNC
                		functions:=.t.
                        endif
                endif
                if k:type==SQL_TAG_LOGICAL .and. j==0
                	if k:oper==SQL_OPER_AND
                        	onlyOr:=.f.
                        endif
                	if k:oper==SQL_OPER_OR
                        	onlyAnd:=.f.
                        endif
                endif
                if k:type==SQL_TAG_GROUP
                	if k:oper==SQL_OPER_GROUPOPEN
                        	j++
                        endif
                	if k:oper==SQL_OPER_GROUPCLOSE
                        	j--
                        endif
                endif
        next
#ifdef DEBUG_FILTERS
	? "filter data="
	for i in data
        	? i
        next
        ? "and=",onlyAnd,"or=",onlyOr, "func=",functions
#endif
        for i in ::from
        	if onlyOr .and. functions .or. onlyMain
                	aadd(ret,"")
                	exit
                endif
        	j:=0 ; s:=""
        	for k in data
                	if k:type==SQL_TAG_GROUP
                             	if k:oper==SQL_OPER_GROUPOPEN
                                	j++
                                endif
                             	if k:oper==SQL_OPER_GROUPCLOSE
                                	j--
                                endif
                                loop
                        endif
                        if j>0
                        	loop
                        endif
                        if k:type==SQL_TAG_FIELD
                        	if k:table==i:alias
                                	s+=iif(k:not,"!","")
                                        /*
                        		s+="__sql_GetValue("+alltrim(str(k:hDB))+;
                            			",'"+k:field+"','"+k:xname+"',x)"+;
                                		iif(onlyAnd," .and. "," .or.  ")
                                        */
                                        s+=k:field
                                        allInOneTable++
                                        loop
                                endif
                        endif
                        if k:type==SQL_TAG_COMPARE
                        	lVal := k:lValue
                        	rVal := k:rValue
                                if rVal:type!=SQL_TAG_CONST .or.;
				   lVal:type!=SQL_TAG_FIELD
                                   	loop
                                endif
                        	if !(lVal:table==i:alias)
                                	loop
                                endif
                                /*
                                lValGet:="__sql_GetValue("+alltrim(str(lVal:hDB))+;
                            			",'"+lval:field+"','"+lVal:xname+"',x)"
                                */
                                lValGet:=lVal:field
                                do case
                                	case k:oper==SQL_OPER_EXACTEQUAL
                                		s+=iif(k:not,"!","")+lValGet+"=="
                                        case k:oper==SQL_OPER_EQUAL
                                		s+=lValGet+iif(k:not,"!=","=")
                                        case k:oper==SQL_OPER_NOTEQUAL
                                		s+=lValGet+iif(k:not,"=","!=")
                                        case k:oper==SQL_OPER_GREAT
                                		s+=lValGet+iif(k:not,"<=",">")
                                        case k:oper==SQL_OPER_GREATEQUAL
                                		s+=lValGet+iif(k:not,"<",">=")
                                        case k:oper==SQL_OPER_LESS
                                		s+=lValGet+iif(k:not,">=","<")
                                        case k:oper==SQL_OPER_LESSEQUAL
                                		s+=lValGet+iif(k:not,">","<=")
                                        otherwise
                                        	loop
                                endcase
                                allInOneTable++
                                s+=rVal:value //+iif(onlyAnd," .and. ","  .or. ")
                        endif
                        if !empty(s)
                		if k:oper == SQL_OPER_AND
                			s+=" .and. "
                		endif
                		if k:oper == SQL_OPER_OR
                			s+=" .or.  "
                		endif
                        endif
                next
                //s:=substr(s,1,len(s)-7)
                aadd(ret,s)
#ifdef DEBUG_FILTERS
                ? "filter",i:alias,"set filter=",s
#endif
        next
        allInOneTable := (len(::from)==1 .and. allInOneTable==len(data))
        /*
        if allInOneTable .and. !onlyMain
        	ret:={}
                asize(ret,len(::from)+1)
                afill(ret,"")
                ret[len(ret)]:={||.t.}
        	return ret
        endif
        */
        *
        // make codeblock as filter for result table
        s:=""
        for k in data
        	if k:type==SQL_TAG_GROUP
                     	if k:oper==SQL_OPER_GROUPOPEN
                        	s+="("
                        endif
                     	if k:oper==SQL_OPER_GROUPCLOSE
                        	s+=")"
                        endif
                        loop
                endif
                if k:type==SQL_TAG_FIELD
                       	s+=iif(k:not,"!","")
                        s+="__sql_GetValue("+alltrim(str(k:hDB))+;
                            ",'"+k:field+"','"+iif(k:xname==NIL,"",k:xname)+"',x)"+;
                       	    iif(onlyAnd," .and. "," .or.  ")
                        loop
                endif

                if k:type==SQL_TAG_COMPARE
                	lVal := k:lValue
                	rVal := k:rValue
                        ss:=""; ss1:=""; ss2:=""
                	if lVal:type==SQL_TAG_FIELD
                        	ss1:="__sql_GetValue("+alltrim(str(lVal:hDB))+;
                            	   ",'"+lVal:field+"','"+iif(lVal:xname==NIL,"",lVal:xname)+"',x)"
                        endif
                	if lVal:type==SQL_TAG_CONST
				ss1=lVal:value
                	endif
                	if lVal:type==SQL_TAG_FUNC
				ss1=lVal:func
                        	fname:=upper(substr(ss1,1,at("(",ss1)-1))
                                if fname $ "COUNT SUM AVG MAX MIN"
					for j in ::fields
                                        	if upper(ss1)==upper(j:fname)
                                                	ss1:='__sql_getValue(NIL,NIL,"'+j:xname+'",x)'
                                                        exit
                                                endif
                                        next
                                endif
                	endif
                	if rVal:type==SQL_TAG_FIELD
                        	ss2="__sql_GetValue("+alltrim(str(rVal:hDB))+;
                            	   ",'"+rVal:field+"','"+iif(rVal:xname==NIL,"",rVal:xname)+"',x)"
                        else // SQL_TAG_CONST
				ss2=rVal:value
                	endif
                        do case
                        	case k:oper==SQL_OPER_EXACTEQUAL
                        		ss=iif(k:not,"!","")+ss1+"=="+ss2
                                case k:oper==SQL_OPER_EQUAL
                        		ss=ss1+iif(k:not,"!=","=")+ss2
                                case k:oper==SQL_OPER_NOTEQUAL
                        		ss=ss1+iif(k:not,"=","!=")+ss2
                                case k:oper==SQL_OPER_GREAT
                        		ss=ss1+iif(k:not,"<=",">")+ss2
                                case k:oper==SQL_OPER_GREATEQUAL
                        		ss=ss1+iif(k:not,"<",">=")+ss2
                                case k:oper==SQL_OPER_LESS
                        		ss=ss1+iif(k:not,">=","<")+ss2
                        	case k:oper==SQL_OPER_PERCENT
                        		ss=iif(k:not,"!","")+ss1+"%"+ss2
                                case k:oper==SQL_OPER_LESSEQUAL
                        		ss=ss1+iif(k:not,">","<=")+ss2
				case k:oper==SQL_OPER_LIKE
                        		ss=iif(k:not,"!","")+"search("+ss2+","+ss1+")"
				case k:oper==SQL_OPER_IN
                                	// make constant
                                	x:=split(rVal:value,",")
                                        y:=map()
                                        for j=1 to len(x)
                                        	x[j]:=&(x[j])
                                                y[ x[j] ]:=x[j]
                                        next
                                        aadd(::constants,y)
                        		//ss=iif(k:not,"!","")+ss1+" $ __self:constants["+alltrim(str(len(::constants)))+"]"
                        		ss=iif(k:not,"!","")+"__sql_run_subQuery(__self:constants["+alltrim(str(len(::constants)))+"],"+ss1+")"
				case k:oper==SQL_OPER_EXIST
                                	y:=makeSqlSubSelect(SQL_OPER_EXIST,ss2,ss1)
                                        aadd(::constants,y)
                        		ss=iif(k:not,"!","")+"__sql_run_subQuery(__self:constants["+alltrim(str(len(::constants)))+"],"+ss1+")"
				case k:oper==SQL_OPER_ALL
                                	y:=makeSqlSubSelect(SQL_OPER_ALL,ss2,ss1)
                                        aadd(::constants,y)
                        		ss=iif(k:not,"!","")+"__sql_run_subQuery(__self:constants["+alltrim(str(len(::constants)))+"],"+ss1+")"
				case k:oper==SQL_OPER_ANY
                                	y:=makeSqlSubSelect(SQL_OPER_ANY,ss2,ss1)
                                        aadd(::constants,y)
                        		ss=iif(k:not,"!","")+"__sql_run_subQuery(__self:constants["+alltrim(str(len(::constants)))+"],"+ss1+")"
				case k:oper==SQL_OPER_SOME
                                	y:=makeSqlSubSelect(SQL_OPER_SOME,ss2,ss1)
                                        aadd(::constants,y)
                        		ss=iif(k:not,"!","")+"__sql_run_subQuery(__self:constants["+alltrim(str(len(::constants)))+"],"+ss1+")"
                                otherwise
                                /*
                                	error ???
                                // between converted to AND
				case k:oper==SQL_OPER_NOTBETWEEN
				case k:oper==SQL_OPER_BETWEEN
                                */
                        endcase
                        s+=ss//+iif(onlyAnd .or. k:oper==SQL_OPER_AND," .and. ","  .or. ")
                endif
                if "OPER" $ k
                	if k:oper == SQL_OPER_AND
                		s+=" .and. "
                	endif
                	if k:oper == SQL_OPER_OR
                		s+=" .or.  "
                	endif
                endif
                //s+=iif(onlyAnd .or. k:oper==SQL_OPER_AND," .and. ","  .or. ")
        next
        //s:=substr(s,1,len(s)-7)
        if empty(s)
        	s:="{|| .t. }"
        else
        	s:="{|x,y|"+s+"}"
        endif
        aadd(ret,&s)
#ifdef DEBUG_FILTERS
        ? "filter for result table=",s
#endif

return ret
