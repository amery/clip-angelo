**************************************************
* SELECT .... FROM .....
******

function fx_select(sQuery)
	local oQuery:=sqlQueryNew(),tmp
	oQuery:parse(sQuery)
	oQuery:parseFrom()
	oQuery:parseTo()
	oQuery:openDB()
	oQuery:parseFields()
	oQuery:parseOrder()
	oQuery:parseGroup()
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
	oQuery:run()
	oQuery:outResult()
	oQuery:closeOut()
	oQuery:closeDB()
return
