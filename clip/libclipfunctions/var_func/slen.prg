*****************************************************
function slen(per)
	local n:=0
	do case
		case stype(per)=="C"
			n=len(&per)
		case stype(per)=="D"
			n=len(dtoc(&per))
		case stype(per)=="N"
		n=len(str(&per))
	endcase
return n
