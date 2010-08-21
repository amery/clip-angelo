STATIC FUNCTION Execute( oTopMenu )
   LOCAL oNewMenu, nCurrent := 0, i, lPas := .T.
   LOCAL nOldRow, nOldCol, nOldCursor

   oNewMenu := oStatus:oMenu:GetItem( oStatus:oMenu:Current )

   IF ( !( ValType( oNewMenu ) == "O" ) )
   ELSEIF ( !oNewMenu:IsPopUp() )
      IF ( oStatus:oMenu:ClassName == "TOPBARMENU" )
	 nOldRow    := oStatus:oldRow
	 nOldCol    := oStatus:oldCol
	 nOldCursor := oStatus:oldCursor
	 SETPOS( oStatus:oldRow, oStatus:oldCol )
	 SETCURSOR( oStatus:oldCursor )
	 nCurrent := oStatus:oMenu:Current
	 EVAL( oNewMenu:Data, oNewMenu )
	 oStatus:oMenu:Select( nCurrent )
	 SETCURSOR( SC_NONE )
	 lPas := .F.
      ELSEIF ( oStatus:oMenu:ClassName == "POPUPMENU" )
	 do while popMenu(oNewMenu)
	 enddo
	 nOldRow    := oStatus:oldRow
	 nOldCol    := oStatus:oldCol
	 nOldCursor := oStatus:oldCursor
	 SETPOS( oStatus:oldRow, oStatus:oldCol )
	 SETCURSOR( oStatus:oldCursor )
	 nCurrent := oStatus:oMenu:Current
	 EVAL( oNewMenu:Data, oNewMenu )
	 oStatus:oMenu:Select( nCurrent )
	 SETCURSOR( SC_NONE )
	 lPas := .F.
      ENDIF

      IF PopMenu( oTopMenu )
	 nCurrent := oStatus:oMenu:Current
      ENDIF

      oStatus:oMenu:Select( nCurrent )
      IF ( oStatus:oMenu:ClassName == "POPUPMENU" .AND. ;
	 oStatus:menuLevel == 1 .AND. !oStatus:oMenu:IsOpen )
	 oStatus:oMenu:Open()
      ENDIF

      IF lPas
	 do while popMenu(oNewMenu)
	 enddo
	 nOldRow    := oStatus:oldRow
	 nOldCol    := oStatus:oldCol
	 nOldCursor := oStatus:oldCursor
	 SETPOS( oStatus:oldRow, oStatus:oldCol )
	 SETCURSOR( oStatus:oldCursor )
	 nCurrent := oStatus:oMenu:Current
	 EVAL( oNewMenu:Data, oNewMenu )
	 oStatus:oMenu:Select( nCurrent )
	 SETCURSOR( SC_NONE )

      ENDIF
      RETURN ( oNewMenu:Id )
   ENDIF
RETURN ( 0 )
