FUNCTION GetPostValidate( oGet, oMsg )

   LOCAL lSavUpdated
   LOCAL lValid := .T.
   LOCAL nCursor

   IF ( oGet:exitState == GE_ESCAPE )
      RETURN ( .T. )
   ENDIF

   IF ( oGet:badDate() )
      oGet:Home()
      DateMsg()
      ShowScoreboard()
      RETURN ( .F. )
   ENDIF

   IF ( oGet:changed )
      oGet:assign()
      oStatus:updated := .T.
   ENDIF
   nCursor := SETCURSOR()
   oGet:reset()
   SETCURSOR( nCursor )

   IF !( oGet:postBlock == NIL )
      lSavUpdated := oStatus:updated
      IF VALTYPE( oGet:Buffer ) == "C"
	 SETPOS( oGet:row, oGet:Col + LEN( oGet:Buffer ) )
      ENDIF
      lValid := EVAL( oGet:postBlock, oGet, oMsg )
      SETPOS( oGet:row, oGet:Col )
      ShowScoreBoard()
      oGet:updateBuffer()
      oStatus:updated := IIF( oGet:changed, .T., lSavUpdated )
      IF ( oStatus:killread )
	 oGet:exitState := GE_ESCAPE
	 lValid := .T.
      ENDIF
   ENDIF
RETURN ( lValid )
