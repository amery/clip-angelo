FUNCTION GUIPostValidate( oGet, oGUI, oMsg )

   LOCAL lSavUpdated
   LOCAL lValid := .T.
   LOCAL uOldData, uNewData

   IF ( oGet:exitState == GE_ESCAPE )
      RETURN ( .T. )
   ENDIF

   IF ( !( oGUI:ClassName() == "TBROWSE" ) )
      uOldData := oGet:VarGet()
      IF ( oGUI:ClassName() $ "LISTBOX_RADIOGROUP" .AND. ;
	 VALTYPE( oGet:VarGet() ) == "N" )
	 uNewData := oGUI:Value
      ELSE
	 uNewData := oGUI:Buffer
      ENDIF
   ENDIF

   IF ( !( uOldData == uNewData ) )
      oGet:VarPut( uNewData )
      oStatus:updated := .T.
   ENDIF

   IF !( oGet:postBlock == NIL )
      lSavUpdated := oStatus:updated
      lValid := EVAL( oGet:postBlock, oGet, oMsg )
      SETPOS( oGet:row, oGet:Col )
      ShowScoreBoard()
      IF ( !( oGUI:ClassName() == "TBROWSE" ) )
	 oGUI:Select( oGet:VarGet() )
      ENDIF
      oStatus:updated := IIF( oGet:changed, .T., lSavUpdated )
      IF ( oStatus:killread )
	 oGet:exitState := GE_ESCAPE
	 lValid := .T.
      ENDIF
   ENDIF
RETURN ( lValid )
