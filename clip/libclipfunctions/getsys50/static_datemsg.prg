static proc DateMsg()
    local nRow, nCol
    if ( Set(_SET_SCOREBOARD) )
      nRow := Row()
      nCol := Col()

      SetPos(SCORE_ROW, SCORE_COL)
      DispOut(MSG_DATE_ERROR)
	  SetPos(nRow, nCol)

      while ( Nextkey() == 0 )
	    sleep(0.1)
      end

      SetPos(SCORE_ROW, SCORE_COL)
      DispOut("            ")
      SetPos(nRow, nCol)
   end

return
