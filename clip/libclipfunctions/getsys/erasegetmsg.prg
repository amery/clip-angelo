FUNCTION EraseGetMsg( oGet, oMsg )
   LOCAL cMsg, nRow := ROW(), nCol := COL()
   LOCAL lmOldState := MSetCursor( .F. )

   IF ( !( EMPTY( oMsg ) ) .AND. oMsg:flag )

      cMsg := IIF( VALTYPE( oGet:Control ) == "O", ;
			    oGet:Control:Message, oGet:Message )

      RestScreen( oMsg:row, oMsg:left, oMsg:row, ;
		  oMsg:right, oStatus:scrvmsg )

   ENDIF
   MSetCursor( lmOldState )
   SETPOS( nRow, nCol )

   RETURN ( NIL )
