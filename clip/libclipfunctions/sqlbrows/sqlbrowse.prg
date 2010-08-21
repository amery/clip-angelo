FUNCTION SQLBrowse(r,top,left,bottom,right,columns,headers,widths,pictures)
	LOCAL cScreen
	LOCAL browse
	LOCAL nCursSave
	LOCAL nKey
	LOCAL lMore
	LOCAL strMsg1 := [<bof>]
	LOCAL strMsg2 := [<eof>]
	LOCAL strMsg3 := [<empty>]
	LOCAL block
	LOCAL i
	LOCAL column

	top	:= IF(top == NIL,1,top)
	left	:= IF(left == NIL,0,left)
	bottom	:= IF(bottom == NIL,MAXROW(),bottom)
	right	:= IF(right == NIL,MAXCOL(),right)
	top	:= MAX(top,0)
	left	:= MAX(left,0)
	bottom	:= MIN(bottom,MAXROW())
	right	:= MIN(right,MAXCOL())

	IF columns == NIL
		columns := ARRAY(r:NFields())
		FOR i := 1 TO r:NFields()
			columns[i] := r:FieldName(i)
		NEXT
	ENDIF
	IF headers == NIL
		headers := ARRAY(r:NFields())
		FOR i := 1 TO r:NFields()
			headers[i] := r:FieldName(i)
		NEXT
	ENDIF

	cScreen := SAVESCREEN()
	@ top, left TO bottom, right DOUBLE

	browse := TBrowseNew(top+2,left+1,bottom-1,right-1)

	FOR i := 1 TO LEN(columns)
		block := r:FieldBlock(r:FieldNo(columns[i]))
		IF r:FieldBinary(columns[i]) .AND. r:FieldType(columns[i]) == "C"
			column := TBColumnNew(headers[i],{|| "<Binary>"})
		ELSE
			column := TBColumnNew(headers[i],block)
		ENDIF
		IF widths != NIL
			column:width := widths[i]
		ENDIF
		IF pictures != NIL
			column:picture := pictures[i]
		ENDIF
		column:type := r:FieldType(columns[i])
		column:_len := r:FieldLen(columns[i])
		column:dec := r:FieldDec(columns[i])
		browse:addColumn(column)
	NEXT
	browse:skipBlock := {|x| Skipper(x,browse)}
	browse:headSep := MY_HEADSEP
	browse:colSep := MY_COLSEP
	browse:colorSpec := SETCOLOR()
	nCursSave := SETCURSOR(SC_NONE)
	lMore := .T.
	FancyColors(browse)
	nCursSave := SetCursor(SC_NONE)
	browse:rowset := r

	DO WHILE lMore

		nKey := 0
		DO WHILE ! browse:stable //.and. nextKey() == 0
			browse:stabilize()
		ENDDO
		while dispcount()>0
			dispend()
		enddo
		dispOutAt(top-1, right-40, MSG_RECORDS+padr(alltrim(str(r:recno()))+"/"+alltrim(str(r:lastrec())),20) )
		IF browse:stable

			IF browse:hitTop .OR. browse:hitBottom
				dispOutAt( top-1, right-10, iif(r:bof(),strMsg1,strMsg2) )
				TONE(125, 0)
			else
				dispOutAt( top-1, right-10, space(max(len(strMsg1),len(strMsg2))) )
			ENDIF
			if nkey==0
				browse:refreshCurrent()
				browse:ForceStable()
				nKey := InKey(0)
				if setkey(nKey)!=NIL
					eval(setkey(nKey),procname(),procline(),readvar())
					loop
				endif
			endif
		ENDIF

		IF nKey == K_ESC
			lMore := .F.
		ELSE
			ApplyKey(browse, nKey)
		ENDIF
	ENDDO
	SETCURSOR(nCursSave)
RETURN
