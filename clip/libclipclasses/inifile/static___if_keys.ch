static function __IF_keys(sPart)
	local i,ret:={},s1:=::checkName(sPart)
	if s1 $ ::data
		ret:=mapkeys(::data[s1])
		for i=1 to len(ret)
			ret[i] := ::__names[ret[i]]
		next
	endif
return ret
