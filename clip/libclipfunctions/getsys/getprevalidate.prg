FUNCTION GetPreValidate( oGet, oMsg )

   LOCAL lSavUpdated
   LOCAL lWhen := .T.
   local oldReadVar

   oldReadVar := set("__readvar",getReadVar(oGet))
   IF !( oGet:preBlock == NIL )
      lSavUpdated := oStatus:updated
      lWhen := EVAL( oGet:preBlock, oGet, oMsg )
      IF !( VALTYPE( oGet:Control ) == "O" ) .AND. !lWhen
	 oGet:Display()
      ENDIF
      ShowScoreBoard()
      oStatus:updated := lSavUpdated
   ENDIF
   IF ( oStatus:killread )
      lWhen := .F.
      oGet:exitState := GE_ESCAPE
   ELSEIF ( !lWhen )
      oGet:exitState := GE_WHEN
   ELSE
      oGet:exitState := GE_NOEXIT
   ENDIF
   set("__readvar",oldReadVar)
RETURN ( lWhen )
