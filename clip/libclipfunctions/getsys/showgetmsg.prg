FUNCTION ShowGetMsg( oGet, oMsg )
   LOCAL cMsg
   LOCAL lmOldState := MSetCursor( .F. )

   IF ( !( EMPTY( oMsg ) ) .AND. oMsg:flag )

      cMsg := IIF( VALTYPE( oGet:Control ) == "O", ;
			    oGet:Control:Message, oGet:Message )

      IF !EMPTY( cMsg )
	 dispOutAt( oMsg:row, oMsg:left, ;
	    PadC( cMsg, oMsg:right - oMsg:left + 1 ), ;
		  oMsg:color )

      ENDIF

   ENDIF
   MSetCursor( lmOldState )

   RETURN ( NIL )
