FUNCTION GetDoSetKey( keyBlock, oGet )

   LOCAL lSavUpdated, lSetKey
   IF ( oGet:changed )
      oGet:assign()
      oStatus:updated := .T.
   ENDIF

   lSavUpdated := oStatus:updated

   lSetKey := EVAL( keyBlock, oStatus:procname, ;
			      oStatus:procline, ReadVar() )

   IF ( !( VALTYPE( lSetKey ) == "L" ) )
      lSetKey := .T.
   ENDIF

   ShowScoreboard()
   oGet:updateBuffer()
   oStatus:updated := lSavUpdated
   IF ( oStatus:killread )
      oGet:exitState := GE_ESCAPE
   ENDIF
RETURN lSetKey
