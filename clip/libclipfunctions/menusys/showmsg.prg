FUNCTION ShowMsg( oMsg, lMode )
   LOCAL nCurrent, lColorFlag, cMsg := NIL
   LOCAL cSaveColor := SetColor()
   LOCAL mlOldState := MSetCursor( .F. )

   IF ( ValType( oStatus:oldMsgFlag ) == "L" .AND.  oStatus:oldMsgFlag )
      RestScreen( oMsg:row, oMsg:left, oMsg:row, oMsg:right, oStatus:scrVmsg )
   ENDIF
   IF lmode
      IF !( lColorFlag    := ( ValType( oMsg:color ) == "C" ) )
	 oMsg:color := GetClrPair( SetColor(), 1 )
      ENDIF

      IF ( ( oMsg:flag ) .AND. ( nCurrent := oStatus:oMenu:Current ) != 0 )
	 IF !EMPTY( cMsg := oStatus:oMenu:GetItem( nCurrent ):Message )
	    @ oMsg:row, oMsg:left ;
	    SAY PadC( cMsg, oMsg:right - oMsg:left + 1 ) ;
	    COLOR oMsg:color
	 ENDIF
      ENDIF
      oStatus:oldMessage := cMsg
      oStatus:oldMsgFlag := oMsg:flag
   ENDIF
   MSetCursor( mlOldState )
RETURN ( .T. )
