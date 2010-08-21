FUNCTION GETINPUT(xSource, nRow, nCol, lSay, xSay, cPic, bValid, bWhen)
LOCAL nOldRow := ROW(), nOldCol := COL(), GetList[1]

  IF nRow = NIL
    nRow := nOldRow
  ENDIF
  IF nCol = NIL
    nCol := nOldCol
  ENDIF
  IF lSay = NIL
    lSay := .F.
  ENDIF

  SetPos(nRow, nCol)

  IF xSay <> NIL
    DispOut(xSay)
    nCol := COL() +1
    SetPos(nRow, nCol)
  ENDIF

  IF xSource = NIL
    RETURN( "" )
  ENDIF

  //GetList[1] := _GET_(xSource, "_GETINPUT", cPic, bValid, bWhen)
  GetList[1] := getnew(nrow,ncol,;
	   {|_1| __field__ iif(_1==NIL,xSource,xSource:=_1 ) __field__ },;
	   xSource, cPic, setcolor(), "_GETINPUT", bValid, bWhen)

  ReadModal(GetList)

  IF lSay
    SetPos(nRow, nCol)
    DispOut(xSource)
  ENDIF

  SetPos(nOldRow, nOldCol)

RETURN( xSource )
