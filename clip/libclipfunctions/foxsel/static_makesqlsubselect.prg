static function makeSqlSubSelect(type,sQuery,value)
	local oQuery:=sqlQueryNew()
        local tmp
        oQuery:subQuery:=.t.
        oQuery:subQueryType:=type
        oQuery:parse(sQuery)
        oQuery:parseFrom()
        oQuery:parseTo()
        oQuery:openDB()
        oQuery:parseFields()
        oQuery:parseGroup()
        oQuery:parseOrder()
        oQuery:where:=oQuery:parseExpr(oQuery:where)
        oQuery:where:=oQuery:logicalOptimizeExpr(oQuery:where)
        tmp:=oQuery:makeFilters(oQuery:where,.f.)
        oQuery:filter:=atail(tmp)
        asize(tmp,len(tmp)-1)
        oQuery:filters:=tmp
        oQuery:having:=oQuery:parseExpr(oQuery:having)
        oQuery:having:=oQuery:logicalOptimizeExpr(oQuery:having)
        tmp:=oQuery:makeFilters(oQuery:having,.t.)
        oQuery:hFilter:=atail(tmp)

        oQuery:header:=.f.
        oQuery:to:=SQL_TO_ANYMAP
return oQuery
