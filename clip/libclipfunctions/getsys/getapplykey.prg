PROCEDURE GetApplyKey( oGet, nKey, GetList, oMenu, oMsg )

   LOCAL cKey
   LOCAL bKeyBlock
   LOCAL MouseRow, MouseColumn
   LOCAL nButton
   LOCAL nHotItem
   LOCAL lSetKey

   IF "SETKEY" $ oget .and. !( ( bKeyBlock := oget:SETKEY( nKey ) ) == NIL )
      IF valtype( nKey:=eval( bKeyBlock, oGet, nKey ) ) != "N" .or. nkey == 0
	 RETURN
      ENDIF
   ENDIF

   IF !( ( bKeyBlock := SETKEY( nKey ) ) == NIL )
      IF ( lSetKey := GetDoSetKey( bKeyBlock, oGet ) )
	 RETURN
      ENDIF
   ENDIF

   IF ( !( GetList == NIL ) .AND. ;
	( ( nHotItem := Accelerator( GetList, nKey, oMsg ) ) != 0 ) )
      oGet:ExitState := GE_SHORTCUT
      oStatus:nextget  := nHotItem
      oStatus:lastexit := GE_SHORTCUT

   ELSEIF ( !( VALTYPE( oMenu ) == "O" ) )
   ELSEIF ( ( nHotItem := oMenu:GetAccel( nKey ) ) != 0 )
      oStatus:menuid := MenuModal( oMenu, nHotItem, ;
      oMsg:row, oMsg:left, oMsg:right, oMsg:color )
      nKey := 0

   ELSEIF ( IsShortCut( oMenu, nKey )  )
      nKey := 0

   ENDIF

   DO CASE
   CASE( nKey == K_UP )
      keyExit:=5
      if oGet:classname=="TEXTGET"
	 oget:up()
      else
	 oGet:exitState := GE_UP
      endif

   CASE( nKey == K_SH_TAB )
      keyExit:=5
      oGet:exitState := GE_UP

   CASE( nKey == K_DOWN )
      keyExit:=2
      if oGet:classname=="TEXTGET"
	 oget:down()
      else
	 oGet:exitState := GE_DOWN
      endif

   CASE( nKey == K_TAB )
      keyExit:=2
      oGet:exitState := GE_DOWN

   CASE( nKey == K_ENTER )
      keyExit:=15
      if oGet:classname=="TEXTGET"
	 oget:insertLine()
      else
	 oGet:exitState := GE_ENTER
      endif

   CASE( nKey == K_ESC )
      keyExit:=12
      IF ( SET( _SET_ESCAPE ) )
	 oGet:Undo()
	 oGet:exitState := GE_ESCAPE
      ENDIF

   CASE( nKey == K_PGUP )
      keyExit:=6
      if oGet:classname=="TEXTGET"
	 oget:pgup()
      else
	 oGet:exitState := GE_WRITE
      endif

   CASE( nKey == K_PGDN )
      keyExit:=7
      if oGet:classname=="TEXTGET"
	 oget:pgdn()
      else
	 oGet:exitState := GE_WRITE
      endif

   CASE( nKey == K_CTRL_HOME )
      keyExit:=15
      oGet:exitState := GE_TOP

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
	 oGet:HitTest( MouseRow, MouseColumn ) ) == HTCLIENT )

	 if oGet:className == "TEXTGET"
		DO WHILE ( oGet:Edit:nLeft+oGet:Edit:ColWin-1 > MouseColumn )
			oGet:Edit:Left()
		ENDDO
		DO WHILE ( oGet:Edit:nLeft+oGet:Edit:ColWin-1 < MouseColumn )
			oGet:Edit:right()
		ENDDO
		DO WHILE ( oGet:Edit:nTop+oGet:Edit:rowWin-1 > MouseRow )
			oGet:Edit:up()
		ENDDO
		DO WHILE ( oGet:Edit:nTop+oGet:Edit:rowWin-1 < MouseRow )
			oGet:Edit:down()
		ENDDO
	 else
		DO WHILE ( oGet:Col + oGet:Pos - 1 > MouseColumn )
			oGet:Left()
			IF oGet:typeOut
				oGet:Home()
				EXIT
			ENDIF
		ENDDO

		DO WHILE ( oGet:Col + oGet:Pos - 1 < MouseColumn )
			oGet:Right()
			IF oGet:typeOut
				oGet:End()
				EXIT
			ENDIF
		ENDDO
	 endif

      ELSEIF !( nButton == HTNOWHERE )
      ELSEIF ( !( GetList == NIL ) .AND. ;
	 HitTest( GetList, MouseRow, MouseColumn, oMsg ) != 0 )
	 oGet:exitstate := GE_MOUSEHIT
	 oStatus:lastexit := GE_MOUSEHIT
      ELSE
	 oGet:exitstate := GE_NOEXIT
      ENDIF

   CASE( nKey == K_UNDO )
      oGet:Undo()

   CASE( nKey == K_HOME )
      oGet:Home()

   CASE( nKey == K_END )
      oGet:End()

   CASE( nKey == K_RIGHT )
      oGet:Right()

   CASE( nKey == K_LEFT )
      oGet:Left()

   CASE( nKey == K_CTRL_RIGHT )
      oGet:wordRight()

   CASE( nKey == K_CTRL_LEFT )
      oGet:wordLeft()

   CASE( nKey == K_BS )
      oGet:BackSpace()

   CASE( nKey == K_DEL )
      oGet:Delete()

   CASE( nKey == K_CTRL_T )
      oGet:delWordRight()

   CASE( nKey == K_CTRL_Y )
      oGet:delEnd()

   CASE( nKey == K_CTRL_BS )
      oGet:delWordLeft()

   CASE( nKey == K_INS )
      SET( _SET_INSERT, !SET( _SET_INSERT ) )
      ShowScoreboard()

   OTHERWISE

      IF ( nKey >= 32 .AND. nKey <= 255 )

	 cKey := CHR( nKey )

	 IF ( oGet:type == "N" .AND. ( cKey == "." .OR. cKey == "," ) )
	    oGet:toDecPos()
	 ELSE

	    IF ( SET( _SET_INSERT ) )
	       oGet:Insert( cKey )
	    ELSE
	       oGet:OverStrike( cKey )
	    ENDIF

	    IF ( oGet:typeOut )
	       IF ( SET( _SET_BELL ) )
		  ?? CHR(7)
	       ENDIF

	       IF ( !SET( _SET_CONFIRM ) )
		  keyExit:=15
		  oGet:exitState := GE_ENTER
	       ENDIF
	    ENDIF

	 ENDIF

      ENDIF

   ENDCASE

RETURN
