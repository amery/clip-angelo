static function __xDataToString(xData)
	local ret:="",i
	switch valtype(xData)
		case "L"
			ret:=iif(xData,".t.",".f.")
		case "C"
			ret:=xData
		case "D"
			ret:="{"+dtoc(xData,"yyyy.mm.dd")+"}"
		case "N"
			ret:=alltrim(str(xData))
		case "A"
			for i=1 to len(xData)
				ret+=alltrim(toString(xData[i]))+","
			next
			ret:=substr(ret,1,len(ret)-1)
		case "U"
	end
return ret
