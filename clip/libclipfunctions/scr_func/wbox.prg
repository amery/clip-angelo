Function wbox(xType)
	local sStyle
	if xType == NIL
		xType := 0
	endif
	if !set(_SET_DISPBOX)
		return wBoxTerm(xType)
	endif
	if valType(xType) != "N"
		return wBoxTerm(xType)
	endif
	xType := xType % 16
	do case
		case xType == 0
			sStyle := WB_0
		case xType == 1
			sStyle := WB_1
		case xType == 2
			sStyle := WB_2
		case xType == 3
			sStyle := WB_3
		case xType == 4
			sStyle := WB_4
		case xType == 5
			sStyle := WB_5
		case xType == 6
			sStyle := WB_6
		case xType == 7
			sStyle := WB_7
		case xType == 8
			sStyle := WB_8
		case xType == 9
			sStyle := WB_9
		case xType == 10
			sStyle := WB_10
		case xType == 11
			sStyle := WB_11
		case xType == 12
			sStyle := WB_12
		case xType == 13
			sStyle := WB_13
		case xType == 14
			sStyle := WB_14
		case xType == 15
			sStyle := WB_15
	endcase
return wBoxTerm(sStyle)
