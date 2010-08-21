STATIC PROCEDURE DateMsg()

   LOCAL nRow
   LOCAL nCol

   IF ( SET( _SET_SCOREBOARD ) )

      nRow := ROW()
      nCol := COL()

      SETPOS( SCORE_ROW, SCORE_COL )
      DISPOUT( NationMsg( _GET_INVD_DATE ) )
      SETPOS( nRow, nCol )

      WHILE ( NEXTKEY() == 0 )
      ENDDO

      SETPOS( SCORE_ROW, SCORE_COL )
      DISPOUT( SPACE( LEN( NationMsg( _GET_INVD_DATE ) ) ) )
      SETPOS( nRow, nCol )
   ENDIF
RETURN
