static function COBRA_queryNew()
	local oQuery:=cobraQueryNew()
	::__queries[oQuery:id] := oQuery
return oQuery
