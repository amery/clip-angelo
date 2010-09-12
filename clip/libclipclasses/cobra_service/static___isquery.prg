static function __isQuery(oQuery)
	if valtype(oQuery) != "O"
		return .f.
	endif
	if !(oQuery:className == "COBRAQUERY")
		return .f.
	endif
return .t.
