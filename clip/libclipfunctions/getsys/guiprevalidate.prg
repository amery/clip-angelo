FUNCTION GUIPreValidate( oGet, oGUI, oMsg )

   LOCAL lSavUpdated
   LOCAL lWhen := .T.

   IF !( oGet:preBlock == NIL )
      lSavUpdated := oStatus:updated
      lWhen := EVAL( oGet:preBlock, oGet, oMsg )
      IF ( !( oGUI:ClassName() == "TBROWSE" ) )
	 oGUI:Display()
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
RETURN ( lWhen )
