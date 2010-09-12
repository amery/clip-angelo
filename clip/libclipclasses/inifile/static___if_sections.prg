static function __IF_sections()
	local i,ret:={},k:=mapKeys(::data)
	for i=1 to len(k)
		aadd(ret, ::__names[k[i]])
	next
return ret
