static function add_Table( caption, border)
local i, j, s, s1, w, t, k, arr, c_repl, e_str, c_str, cell, ctbl, celine
local tcolor, wfact, rl, symb, x, y, dc, rc, rtbl, elem, hcls, ls, spos
local cell_pos:={}
local box:={"�","�","�","�","�","�","�","�","�","�","�"}
      //     1   2   3   4   5   6   7   8   9   0   1
	border := iif(border==NIL, .f., .t.)
	caption := iif(caption==NIL, "", caption)

	// write caption
	s := box[1]
	ctbl := ::tbl[::numTable]

	for i=1 to len(ctbl[1])
		w := ctbl[1][i]:WIDTH
		for k=2 to ctbl[1][i]:COL
			w += ctbl[1][i+k-1]:WIDTH
		next
		w += ctbl[1][i]:COL-1
		i += (ctbl[1][i]:COL)-1

		s += box[2]:replicate(w)+box[3]
	next
	s := s:left(s:len()-1) + box[4]
	if !empty(caption)
		j := len(s)
		arr := split_string(caption, j)
		for i=1 to len(arr)
			::doc:Text(padc(arr[i], j),,,,,::clr)
		next
	endif
	tcolor := ::clr // ���� �������
	::doc:Text(s,,,,,tcolor)
	::newLine(, .t.)
	// write body
	c_repl := .f.
	for i=1 to len(ctbl)
		// ����� ������
		ls := box[5]
		::doc:Text(ls,,,,,tcolor)
		// ������ ����� � ������
		hcls := 1
		// ��������� ������ �������
		rtbl := ::doc:n_Line
		rc := ::doc:n_Pos
		for j=1 to len(ctbl[i])
			elem := ctbl[i][j]
			w := elem:WIDTH //������ ������
			if len(cell_pos) < j
				aadd(cell_pos, rc)
			else
				cell_pos[j] := rc
			endif
			// ������ ������� �����
			if j==1
				s1 := symb_tbl(ctbl, i, j-1)
			endif
			s1 += replicate(iif(elem:ROW>1 .and. i!=len(ctbl), " ", box[2]), elem:WIDTH)
			// ������ ������� ������/�����, ���� col > 1
			for k=2 to elem:COL
				w += ctbl[i][j+k-1]:WIDTH
				s1 += symb_tbl(ctbl, i, j+k-2)
				s1 += replicate(iif(elem:ROW>1 .and. i!=len(ctbl), " ", box[2]), ctbl[i][j+k-1]:WIDTH)
				if len(cell_pos) < j+k-1
					aadd(cell_pos, rc)
				else
					cell_pos[j+k-1] := rc
				endif
			next
			j += elem:COL-1
			w += elem:COL-1

			// ���������� ������ � ������ ������
			if !"ALIGN"$elem
				elem:ALIGN := ""
			endif

			// ������ ���� ����� ������
			// ���� ������ ������
			if !("text"$elem) .or. len(elem:text)==0
				::doc:Text(" ":replicate(w),rtbl,rc,,,tcolor)
				::doc:refresh()
				for x=1 to hcls
					dc := -1
					for y=1 to j
						if ctbl[i][y]:high <= hcls
							ctbl[i][y]:high ++
							symb := iif(y==1 .or. ctbl[i][y-1]:COL<=1, box[5], " ")
							::doc:Text(symb,rtbl,rc+dc,,,tcolor)
				::doc:refresh()
							symb := iif(y==len(ctbl[i]) .or. ctbl[i][y]:COL<=1, box[5], " ")
							::doc:Text(symb,rtbl,rc+dc+ctbl[i][y]:WIDTH+1,,,tcolor)
				::doc:refresh()
							dc += ctbl[i][y]:WIDTH+1
						endif
					next
				next
				//highs[j] := hcls
				rc += w+1
				s1 += symb_tbl(ctbl, i, j)
				loop
			endif
			// ���� �� ������ ������
			rl := 0

			do while (len(elem:text) > 0)
				// ������� ��������� ������ � ���� ������ ������
				wfact := 0 // ����������� ������ ������ � �������
				y := 0
				for x=1 to len(elem:text)
					if wfact +elem:text[x][2] <= w
						y++
						wfact += elem:text[x][2]
					elseif wfact == 0 .and. elem:text[x][2] > w

						y++
						asize(elem:text, len(elem:text)+1)
						ains(elem:text, x)
						elem:text[x] := {"", 0}
						spos := splitSpaceFrom(elem:text[x+1][1], w-wfact)
						elem:text[x][1] := substr(elem:text[x+1][1], 1, spos-1)
						elem:text[x+1][1] := ltrim(substr(elem:text[x+1][1], spos))
						elem:text[x][2] := len(elem:text[x][1])
						elem:text[x+1][2] := len(elem:text[x+1][1])
						wfact += elem:text[x][2]
						//elem:high ++

						exit
					endif
				next
				::doc:n_Line := rtbl+rl
				::doc:n_Pos := rc
				for x=1 to y
					if valtype(elem:text[x][1])=="O"
						::parseTag(elem:text[x][1])
					else
						::doc:Text(elem:text[x][1], ::doc:n_Line, ::doc:n_Pos,,,::clr)
				::doc:refresh()
					endif
				next
				for x=1 to y
					adel(elem:text, 1)
				next
				asize(elem:text, len(elem:text)-y)
				rl++
			enddo
			/*
			if rl>hcls
				hcls := rl
			endif
			*/
			//highs[j] := rl
			//for x=rl to hcls
			for x=hcls to rl
				dc := -1
				for y=1 to j
					if ctbl[i][y]:high <= rl
						ctbl[i][y]:high ++
						symb := iif(y==1 .or. ctbl[i][y-1]:COL==1, box[5], " ")
						//::doc:Text(symb,rtbl+x-1,rc+dc,,,tcolor)
						::doc:Text(symb,rtbl+x-1,cell_pos[y]-1,,,tcolor)
						::doc:refresh()
						symb := iif(y==len(ctbl[i]) .or. ctbl[i][y]:COL==1, box[5], " ")
						//::doc:Text(symb,rtbl+x-1,rc+dc+ctbl[i][y]:WIDTH+1,,,tcolor)
						::doc:Text(symb,rtbl+x-1,cell_pos[y]+ctbl[i][y]:WIDTH,,,tcolor)
						::doc:refresh()
						dc += ctbl[i][y]:WIDTH+1
					endif
				next
			next
			if rl>hcls
				hcls := rl
			endif
			rc += w+1
			s1 += symb_tbl(ctbl, i, j)
		next
		::doc:n_Line := rtbl+hcls
		::doc:n_Pos := ::doc:marginleft
		::doc:Text(s1,::doc:n_Line,::doc:n_Pos,,,tcolor)
				::doc:refresh()
	next
return
