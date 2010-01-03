static function smtp_addField(sName,sData)
	if valtype(sName)!="C" .or. valtype(sData)!="C"
		return .f.
	endif
	aadd(::fields,{sName,sData})
return .t.
