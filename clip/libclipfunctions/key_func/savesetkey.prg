function SaveSetKey()
	local i,ret:=map()
	for i=MIN_KEY_CODE to MAX_KEY_CODE
		ret[i]:=setkey(i)
	next
return ret
