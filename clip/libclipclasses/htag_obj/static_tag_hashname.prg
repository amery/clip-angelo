static function tag_hashname(hashstr)
	local fff
	if valtype(hashstr)=="C"
		fff:=alltrim(upper(hashstr))
	else
		fff:=hashstr
	endif
	if fff $ ::hashes
		return ::hashes[fff]
	endif
return hashstr
