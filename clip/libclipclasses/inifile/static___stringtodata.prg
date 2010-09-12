static function __stringToData(str)
	local ret:=NIL, err, bErr:=errorblock({|x|break(x)})

	str:=alltrim(str)

	begin sequence
		ret:=&("{"+str+"}")
	recover using err
	end sequence
	// str -> 1,2,3,4,5
	if valType(ret)=="A" .and. len(ret) > 1
		errorBlock(bErr)
		return ret
	endif
	begin sequence
		ret:=&str
	recover using err
	end sequence
	// str -> "string_value"
	if valType(ret) $ "CNDL"
		errorBlock(bErr)
		return ret
	endif
	// str -> "{|| block_expr}"
	if valType(ret) $ "B"
		errorBlock(bErr)
		return str
	endif
	// str -> {yyyy.mm.dd}
	if left(str,1)=="{" .and. right(str,1)=="}"
		errorBlock(bErr)
		ret:=ctod(substr(str,2,len(str)-2),"yyyy.mm.dd")
		return ret
	endif
	// str -> a1,a2,a3,a4
	if ("," $ str) .and. !('"' $ str) .and. !("'" $ str)
		errorBlock(bErr)
		return split(str,",")
	endif
	errorBlock(bErr)
return alltrim(str)
