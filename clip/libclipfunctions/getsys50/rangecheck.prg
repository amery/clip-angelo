func RangeCheck(get, junk, lo, hi)

   local cMsg, nRow, nCol
   local xValue
   if ( !get:changed )
	return (.t.)
   end
   xValue := get:VarGet()
   if ( xValue >= lo .and. xValue <= hi )
	    return (.t.)
   end
   if ( Set(_SET_SCOREBOARD) )
	 cMsg := MSG_RANGE_ERROR + Ltrim(Transform(lo, "")) + ;
		" - " + Ltrim(Transform(hi, ""))
	if ( Len(cMsg) > MaxCol() )
		cMsg := Substr( cMsg, 1, MaxCol() )
	end
	nRow := Row()
	nCol := Col()
	SetPos( SCORE_ROW, Min(60, MaxCol() - Len(cMsg)) )
	DispOut(cMsg)
	SetPos(nRow, nCol)

	while ( NextKey() == 0 )
	      sleep(0.1)
	end

	SetPos( SCORE_ROW, Min(60, MaxCol() - Len(cMsg)) )
	DispOut( Space(Len(cMsg)) )
	SetPos(nRow, nCol)
   end
return (.f.)
