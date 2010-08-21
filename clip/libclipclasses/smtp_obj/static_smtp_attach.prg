static function smtp_attach(sFile)
	if valtype(sFile)!="C"
		return .f.
	endif
	aadd(::attachments,sFile)
return .t.
