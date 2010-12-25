static function parse_Table(tag, widthtab)
local i, j, tt, col, row, cnt:=1, align, caption := "", border, width
local arr:={}, isCell, ind, maxwidth:={}, minwidth:={}, ct, fact, mmn, k
local nCol, item
/*
tblCol - columns array:
	minWidth
	maxWidth
	realWidth

tblRow - rows array:
	height

tbl - items array
	body - array:{item , len(item)} item  - text or html's tag
*/

	widthtab := iif(widthtab==NIL, 0, widthtab)
	width := 0
	if "WIDTH"$tag:fields
		k:="%":at(tag:fields:WIDTH)
		if k>0
			k := &(substr(tag:fields:WIDTH, 1, k-1))
			width := int(widthtab*k)/100
		else
			width:=&(tag:fields:WIDTH)/8
		endif
	endif
	border := .f.
	if "BORDER"$tag:fields
		border := .t.
	endif
	aadd(::tbl, HTableNew(__self__, width, border))
	::numTable++
	isCell := .f.
	//aadd(::tblRow, {})
	//aadd(::tblCol, {})
	do while !::parser:empty()
		tt := ::parser:get()
		if empty(tt)
			loop
		endif
		if valtype(tt)=="C" .and. isCell
			// add to table new cell string
			::tbl[::numTable]:addItemToCell(tt)
			loop
		endif
		if tt:tagname == "CAPTION"
		     if "ALIGN" $ (tt:fields)
			::tbl[::numtable]:captionAlign := tt:fields:ALIGN
		     endif
		     ::tbl[::numTable]:caption := ::parse_Tbl_Caption(@tt)
		endif
		do case
		case tt:tagname == "TABLE"
			cnt ++
			aadd(arr, {})
		case tt:tagname == "/TABLE"
			::numTable--
			isCell := .f.
			if ::numTable == 0
				exit
			endif
		case tt:tagname == "TR"
			isCell := .f.
			::tbl[::numTable]:addRowToTable(tt)
		case tt:tagname == "TH" .or. tt:tagname == "TD"
			::tbl[::numTable]:addCellToTable(tt)
			isCell := .t.
		case tt:tagname == "/TH" .or. tt:tagname == "/TD" .or. tt:tagname == "/TR"
			isCell := .f.
		otherwise
			if tt:tagname != "/CAPTION"
				::tbl[::numTable]:addItemToCell(tt, .t.)
			endif
		endcase
		if (valtype(tt) == "O" .and. (tt:tagname == "/TABLE")) .and. ::numTable == 1
			exit
		endif
	enddo

	::numTable := 1
	item := ::tbl[::numTable]

	width -= len(::tbl[::numTable]:TColumns)-1

	// ���� ������� ������ �������
	if width>0
		// ������ ������
		fact := 0
		for i=1 to len(item:Tcolumns)
			fact += item:Tcolumns[i]
		next
		for j=1 to len(item:Tcolumns)
			tt := int((item:Tcolumns[j]*100)/fact)
			item:Tcolumns[j] := int((width*tt)/100)
		next
	endif

	k := 0
	for i=1 to len(item:Tcolumns)
		k += item:Tcolumns[i]
	next
	if width<0 .and. k>(::width-k+1)
		width := ::width-len(item:Tcolumns)+1
	else
		width := k + len(item:Tcolumns) + 1
	endif

	j := 0
	k := len(item:Tcolumns)
	for i=1 to k
		if item:Tcolumns[i]>int(width/k)
			item:Tcolumns[i] := (int(i*width/k))-j
		endif
		j += item:Tcolumns[i]
	next
	::tbl[::numTable]:drawTable()//::doc, ::clr)
	::numTable --
/*
	for i=1 to len(item:rows)
		for j=1 to len(item:Tcolumns)
			? 'items   ', item:items[1][j]:items
			? 'maxWidth', item:items[1][j]:maxWidth
			? 'minWidth', item:items[1][j]:minWidth
			? 'Width   ', item:items[1][j]:Width
			? 'colspan ', item:items[1][j]:colspan
			? 'rowspan ', item:items[1][j]:rowspan
			? 'Height  ', item:items[1][j]:Height

		next
		? '-------------------------------------------------'
		exit
	next
*/
//	::add_table(caption)
return .t.
