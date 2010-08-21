PROCEDURE TBApplyKey( oGet, oTB, GetList, nKey, oMenu, oMsg )

   LOCAL cKey
   LOCAL bKeyBlock
   LOCAL MouseRow, MouseColumn
   LOCAL nButton
   LOCAL nHotItem
   LOCAL lSetKey

   IF "SETKEY" $ oget .and.  !( ( bKeyBlock := oget:SETKEY( nKey ) ) == NIL )
      IF ( ( nKey:=eval( bKeyBlock, oGet, nKey ) ) == 0 )
	 RETURN
      ENDIF
   ENDIF
   IF !( ( bKeyBlock := SETKEY( nKey ) ) == NIL )
      IF ( lSetKey := GetDoSetKey( bKeyBlock, oGet ) )
	 RETURN
      ENDIF
   ENDIF

   IF ( ( nHotItem := Accelerator( GetList, nKey, oMsg ) ) != 0 )
      oGet:ExitState := GE_SHORTCUT
      oStatus:nextget := nHotItem

   ELSEIF ( !( VALTYPE( oMenu ) == "O" ) )
   ELSEIF ( ( nHotItem := oMenu:GetAccel( nKey ) ) != 0 )
      oStatus:menuid := MenuModal( oMenu, nHotItem, ;
      oMsg:row, oMsg:left, oMsg:right, oMsg:color )
      nKey := 0

   ELSEIF ( IsShortCut( oMenu, nKey )  )
      nKey := 0

   ENDIF

   DO CASE
   CASE( nKey == K_TAB )
	 keyExit:=2
      oGet:exitState := GE_DOWN

   CASE( nKey == K_SH_TAB )
	 keyExit:=5
      oGet:exitState := GE_UP

   CASE( nKey == K_ENTER )
	 keyExit:=15
      if oGet:classname=="TEXTGET"
	 oget:insertLine()
      else
	 oGet:exitState := GE_ENTER
      endif
      //oGet:exitState := GE_ENTER

   CASE( nKey == K_ESC )
	 keyExit:=12
      IF ( SET( _SET_ESCAPE ) )
	 oGet:exitState := GE_ESCAPE
      ENDIF

#ifdef CTRL_END_SPECIAL
   CASE( nKey == K_CTRL_END )
	 keyExit:=14
      oGet:exitState := GE_BOTTOM
#else
   CASE( nKey == K_CTRL_W )
	 keyExit:=14
      oGet:exitState := GE_WRITE
#endif
   CASE( ( nKey == K_LBUTTONDOWN ) .OR. ( nKey == K_LDBLCLK ) )
      MouseRow    := mROW()
      MouseColumn := mCOL()

      IF ( !( VALTYPE( oMenu ) == "O" ) )
	 nButton := 0

      ELSEIF ( !( oMenu:ClassName() == "TOPBARMENU" ) )
	 nButton := 0

      ELSEIF ( ( nButton := oMenu:HitTest( MouseRow, MouseColumn ) ) != 0 )
	 oStatus:menuid := MenuModal( oMenu, nHotItem, ;  // Changed.
	 oMsg:row, oMsg:left, oMsg:right, oMsg:color )
	 nButton := 1

      ENDIF

      IF ( nButton != 0 )
      ELSEIF ( ( nButton := ;
	 oTB:HitTest( MouseRow, MouseColumn ) ) == HTNOWHERE )
	 IF ( HitTest( GetList, MouseRow, MouseColumn, oMsg ) != 0 )
	    oGet:exitstate := GE_MOUSEHIT
	    oStatus:lastexit := GE_MOUSEHIT
	 ELSE
	    oGet:exitstate := GE_NOEXIT
	 ENDIF
      ENDIF
   ENDCASE

   RETURN
