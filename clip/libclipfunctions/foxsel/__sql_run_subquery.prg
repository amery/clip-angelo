function __sql_run_subQuery(data,value)
	local m, oper:=SQL_OPER_IN
        local i,ret:=.f.
	if data==NIL .or. empty(data)
        	return ret
        endif
        if valtype(data)=="O"
        	if "CLASSNAME" $ data .and. data:classname=="SQLSELECT"
                	data:run()
                	m:=data:outResult()
                        oper:=data:subQueryType
                else
                	m:=data
                endif
        else
        	return value==data
        endif
        do case
        	case oper==SQL_OPER_IN
                	ret:= ( value $ m)
                	for i in m
                        	if i==value
                                   	ret:=.t.
                                        exit
                                endif
                        next
        	case oper==SQL_OPER_ANY
                	ret:= ( value $ m)
                	for i in m
                        	if i==value
                                   	ret:=.t.
                                        exit
                                endif
                        next
        	case oper==SQL_OPER_SOME
                	ret:= ( value $ m)
                	for i in m
                        	if i==value
                                   	ret:=.t.
                                        exit
                                endif
                        next
        	case oper==SQL_OPER_EXIST
                	ret:= empty(m)
        	case oper==SQL_OPER_ALL
                	ret:=.t.
                        for i in m
                        	if !(i==value)
                                	ret:=.f.
                                        exit
                                endif
                        next
        endcase
return ret
