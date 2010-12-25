static func addScroll( x, col )
	local str:="", len, xType, mmm
	xType=valType(x)
	do case
		case xType=="L"
			len= 3
		case xType=="D"
			len= len( dtoc(x) )
		case xType=="N"
			len= len( str(x) )
		otherwise             // "C" | "M"
			len= len(x)
	endcase
	if len+col > (mmm:=mset( _MSET_RIGHTBANK ))
		str="@S"+alltrim(str(mmm-col+1))
	endif
return str
