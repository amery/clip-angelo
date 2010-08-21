function symb_tbl(arr, row, col)
local symb:=" ", elem, elemD, elemR, len
local box:={"┌","─","┬","┐","│","├","┤","┼","└","┴","┘"}
      //     1   2   3   4   5   6   7   8   9   0   1
	len := len(arr[row])
	elem := arr[row][max(1, min(col, len))]
	if len(arr)>row
		elemD := arr[row+1][max(1, min(col, len(arr[row+1])))]
	endif
	if col < len
		elemR := arr[row][col+1]
	endif
	if row<len(arr)
		if col==0
			symb := iif(elem:rowspan>1, box[5], box[6])
		elseif col>=len
			symb := iif(elem:colspan!=1 .or. elem:rowspan>1, box[5], box[7])
/*
		elseif col==1 .and. elem:colspan==1
			symb := iif(elem:rowspan>1, box[6], box[8])
			if elemD != NIL .and. elemD:colspan>1
				symb := box[10]
			endif
*/
		elseif col<len .and. elem:colspan>1 .and. row<len(arr) .and. elem:rowspan==1
			symb := box[3]
		elseif col>=1 .and. col<len
			symb := iif(elem:rowspan==1, iif(elem:colspan==1, box[8], box[6]), box[6])
			if elemR !=NIL .and. elemR:rowspan > 1
				symb := box[7]
			endif
			if elemD != NIL .and. elemD:colspan>1
				if elem:colspan>1
					symb := " "
				else
					symb := box[10]
				endif
			endif

		endif
		return symb
	endif
	if row==len(arr)
		if col==0
			symb := box[9]
		elseif col==1
			symb := iif(elem:colspan==1, box[10], box[2])//box[9]
		elseif col<len
			symb := iif(elem:colspan==1, box[10], box[2])
		else
			symb := box[11]
		endif
	endif
return symb
