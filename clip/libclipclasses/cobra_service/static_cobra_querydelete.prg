static function COBRA_queryDelete(nQuery)
	if valtype(nQuery) != "N"
		return .f.
	endif
	adel(::__queries,nQuery)
return .t.
