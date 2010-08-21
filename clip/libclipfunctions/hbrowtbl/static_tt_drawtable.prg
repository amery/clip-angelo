static function tt_drawTable()//Doc, Color) //::doc, ::clr
local i, j, s, s1, w, t, k, arr, c_repl, e_str, c_str, cell, ctbl, celine
local tcolor, wfact, symb, x, y, dc, elem, spos, kx,dataOk
local box:={"┌","─","┬","┐","│","├","┤","┼","└","┴","┘"}
      //     1   2   3   4   5   6   7   8   9   0   1

	// write caption
	s := box[1]

	for i=1 to len(::Tcolumns)
		w := ::Tcolumns[i]
		for k=2 to ::Titems[1][i]:colspan
			w += ::Tcolumns[i+k-1]
		next
		w += ::Titems[1][i]:colspan-1
		i += (::Titems[1][i]:colspan)-1

		s += box[2]:replicate(w)+box[3]
	next
	s := s:left(s:len()-1) + box[4]
	if !empty(::caption) .and. upper(::captionAlign) == "TOP"
		j := len(s)
		arr := split_string(::caption, j)
		for i in arr
			::doc:Text(padc(i, j),,,,,::clr)
		next
	endif
	tcolor := ::clr // цвет таблицы
	::doc:n_Pos := ::doc:marginLeft
	::doc:Text(s,,,,,tcolor)

	//::newLine(, .t.)
	::doc:n_Line ++
	::doc:n_Pos := ::doc:marginLeft
	::posInTable := ::doc:marginLeft
	// write body
	c_repl := .f.
	for i=1 to len(::Titems)
		::doc:n_Line := ::doc:lines+1
		::doc:n_Pos := ::posInTable
		::TblNewLine := .f.
		do while .t.
		dataOk := .t.
		for j=1 to len(::Titems[i])
			elem := ::Titems[i][j]
			if elem:colspan>1
				elem:items := ::Titems[i][j+elem:colspan-1]:items
			endif

			w := ::Tcolumns[j] //ширина ячейки
			// нижняя граница ячеек
			if j==1
				s1 := symb_tbl(::Titems, i, j-1)
			endif
			s1 += replicate(iif(elem:rowspan>1 .and. i!=len(::Titems), " ", box[2]), ::Tcolumns[j])
			// ширина текущей ячейки/ячеек, если col > 1
			for k=2 to elem:colspan
				w += ::Tcolumns[j+k-1]
				s1 += symb_tbl(::Titems, i, j+k-2)
				s1 += replicate(iif(elem:rowspan>1 .and. i!=len(::Titems), " ", box[2]), ::Tcolumns[j+k-1])
			next
			j += elem:colspan-1
			w += elem:colspan-1

			wfact := 0 // фактическая ширина n-столбцов
			y := 0
			for x=1 to len(elem:items)
				if ::tblNewLine
					exit
				endif
				if wfact +elem:items[x][2] <= w
					y++
					wfact += elem:items[x][2]
				elseif wfact == 0 .and. elem:items[x][2] > w

					y++
					asize(elem:items, len(elem:items)+1)
					ains(elem:items, x)
					elem:items[x] := {"", 0}
					spos := splitSpaceFrom(elem:items[x+1][1], w-wfact)
					elem:items[x][1] := substr(elem:items[x+1][1], 1, spos-1)
					elem:items[x+1][1] := ltrim(substr(elem:items[x+1][1], spos))
					elem:items[x][2] := len(elem:items[x][1])
					elem:items[x+1][2] := len(elem:items[x+1][1])
					wfact += elem:items[x][2]

					exit
				endif
			next
			if j-elem:colspan == 0
				::doc:Text(iif(::border, box[5], " "),::doc:n_Line, ::doc:n_Pos,,,tcolor)
			endif
			kx := 0
			for x=1 to y
				if valtype(elem:items[x][1])=="O"
					::parseTag(elem:items[x][1])
					kx := x
					loop
				endif
				if !::tblNewLine .and. len(elem:items)>0
					do switch (elem:align)
					case HASH_LEFT
						::addObject(padr(elem:items[x][1], w), ::clr,,,,,::doc:n_Line, ::doc:n_Pos)
					case HASH_RIGHT
						::addObject(padl(elem:items[x][1], w), ::clr,,,,,::doc:n_Line, ::doc:n_Pos)
					case HASH_CENTER
						::addObject(padc(elem:items[x][1], w), ::clr,,,,,::doc:n_Line, ::doc:n_Pos)
					endswitch
				endif
			next
			if ::tblNewLine
				::tblNewLine := .f.
				y := kx // first elements to newLine
			endif
			for x=1 to y
				adel(elem:items, 1)
			next
			asize(elem:items, len(elem:items)-y)
			dataOk := dataOk .and. empty(elem:items)
			::doc:Text(iif(::border, box[5], " "), ::doc:n_Line, ::doc:n_Pos+iif(y==0, w, 0),,,tcolor)

			s1 += symb_tbl(::Titems, i, j)
		next
		::doc:n_Line ++
		::doc:n_Pos := ::posInTable
		if dataOk
			exit
		endif

		enddo
		::doc:Text(s1,::doc:n_Line,::doc:n_Pos,,,tcolor)
	next
	if !empty(::caption) .and. upper(::captionAlign) == "BOTTOM"
		j := len(s)
		arr := split_string(::caption, j)
		for x in arr
			::doc:Text(padc(x, j),,,,,::clr)
		next
	endif
return
