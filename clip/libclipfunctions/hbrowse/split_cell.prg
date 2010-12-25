function split_cell(cell, width)
local arr:={}, i, s:="", es:=""
	for i=1 to len(cell)
		if len(s+es)+cell[i][2]>width
			arr:aadd(s)
			s += es+iif(valtype(cell[i][1])=="O", " ":replicate(cell[i][2]), cell[i][1])
		else
			s += es+iif(valtype(cell[i][1])=="O", " ":replicate(cell[i][2]), cell[i][1])
		endif
		es := " "
	next
	arr:aadd(s)
return arr
