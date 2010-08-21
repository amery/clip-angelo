STATIC PROCEDURE ShowScoreboard()

   LOCAL nRow
   LOCAL nCol

   IF ( SET( _SET_SCOREBOARD ) )
      nRow := ROW()
      nCol := COL()
      SETPOS( SCORE_ROW, SCORE_COL )
      DISPOUT( IIF( SET( _SET_INSERT ), NationMsg( _GET_INSERT_ON ), ;
					NationMsg( _GET_INSERT_OFF ) ) )
      SETPOS( nRow, nCol )
   ENDIF
RETURN
