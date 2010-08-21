PROCEDURE GUIApplyKey( oGet, oGUI, GetList, nKey, oMenu, oMsg )

   LOCAL cKey
   LOCAL bKeyBlock
   LOCAL MouseRow, MouseColumn
   LOCAL nButton
   LOCAL TheClass
   LOCAL nHotItem
   LOCAL lClose
   LOCAL lSetKey

   IF "SETKEY" $ oget .and.  !( ( bKeyBlock := oget:SETKEY( nKey ) ) == NIL )
      IF valtype( nKey:=eval( bKeyBlock, oGet, nKey ) ) != "N" .or. nkey == 0
	 RETURN
      ENDIF
   ENDIF
   IF ( !( bKeyBlock := SETKEY( nKey ) ) == NIL )
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

#ifdef __CLIP__
   if nkey==K_LEFT
      oGet:exitState := GE_UP
      nkey:=0
   endif
   if nkey==K_RIGHT
      oGet:exitState := GE_DOWN
      nkey:=0
   endif
#endif

   IF ( nKey == 0 )
   ELSEIF ( ( TheClass := oGUI:ClassName() ) == "RADIOGROUP" )
      IF ( nKey == K_UP )
	 keyExit:=5
	 oGUI:PrevItem()
	 nKey := 0

      ELSEIF ( nKey == K_DOWN )
	 keyExit:=2
	 oGUI:NextItem()
	 nKey := 0

      ELSEIF ( ( nHotItem := oGUI:GetAccel( nKey ) ) != 0 )
	 oGUI:Select( nHotItem )

      ENDIF

      IF VALTYPE( oGet:VarGet() ) == "N"
	 oGet:VarPut( oGui:Value )
      ENDIF

   ELSEIF ( TheClass == "CHECKBOX" )
      IF ( nKey == K_SPACE )
	 oGUI:Select()
      ENDIF

   ELSEIF ( TheClass == "PUSHBUTTON" )
      IF ( nKey == K_SPACE )
	 oGUI:Select( K_SPACE )

      ELSEIF ( nKey == K_ENTER )
	 keyExit:=15
	 oGUI:Select()
	 nKey := 0
      ENDIF

   ELSEIF ( TheClass == "LISTBOX" )
      IF ( nKey == K_UP )
	 keyExit:=5
	 if oGui:isOpen .or. oGui:useArrow
		oGUI:PrevItem()
	 else
		oGet:exitState := GE_UP
	 endif
	 nKey := 0

      ELSEIF ( nKey == K_DOWN )
	 keyExit:=2
	 if oGui:isOpen .or. oGui:useArrow
		oGUI:NextItem()
	 else
		oGet:exitState := GE_DOWN
	 endif
	 nKey := 0

      ELSEIF ( nKey == K_SPACE )
	 IF ( !oGUI:DropDown )
	 ELSEIF ( !oGUI:IsOpen )
	    oGUI:Open()
	    nKey := 0
	 ENDIF

      ELSEIF ( nKey == K_ENTER .and. oGui:isOpen )
		oGui:select()
		oGui:close()
		nkey:=0

      ELSEIF ( nKey == K_ESC .and. oGui:isOpen )
		oGui:close()
		nkey:=0

      ELSEIF ( nKey == K_PGUP .and. oGui:isOpen )
		oGui:prevPage()
		nkey:=0

      ELSEIF ( nKey == K_PGDN .and. oGui:isOpen )
		oGui:nextPage()
		nkey:=0

      ELSEIF ( ( nButton := oGUI:FindText( CHR( nKey ), oGUI:Value + 1, ;
					   .F., .F. ) ) != 0 )
	 oGUI:Select( nButton )

      ENDIF
      IF VALTYPE( oGet:VarGet() ) == "N"
	 oGet:VarPut( oGui:Value )
      ENDIF

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
      //oGet:exitState := GE_ENTER

   CASE( nKey == K_ESC )
	 keyExit:=12
      IF ( SET( _SET_ESCAPE ) )
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

      lClose := .T.

      IF ( nButton != 0 )
      ELSEIF ( ( nButton := ;
	 oGUI:HitTest( MouseRow, MouseColumn ) ) == HTNOWHERE )
	 IF ( HitTest( GetList, MouseRow, MouseColumn, oMsg ) != 0 )
	    oGet:exitstate := GE_MOUSEHIT
	    oStatus:lastexit := GE_MOUSEHIT
	 ELSE
	    oGet:exitstate := GE_NOEXIT
	 ENDIF

      ELSEIF ( nButton >= HTCLIENT )
	 oGUI:Select( nButton )

      ELSEIF ( nButton == HTDROPBUTTON )
	 IF ( !oGUI:IsOpen )
	    oGUI:Open()
	    lClose := .F.

	 ENDIF

      ELSEIF ( ( nButton >= HTSCROLLFIRST ) .AND. ;
	       ( nButton <= HTSCROLLLAST ) )
	 oGUI:Scroll( nButton )
	 lClose := .F.

      ENDIF

      IF ( !lClose )
      ELSEIF !( TheClass == "LISTBOX" )
      ELSEIF ( !oGUI:DropDown )
      ELSEIF ( oGUI:IsOpen )
	 oGUI:Close()
	 oGUI:Display()
      ENDIF

   ENDCASE

RETURN
