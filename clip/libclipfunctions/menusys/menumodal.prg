FUNCTION MenuModal( oTopMenu, nSelection, nMsgRow, nMsgLeft, nMsgRight, ;
		    cMsgColor, GetList )
   LOCAL k,nKey, nNewItem, lSaveCursor:=setCursor(), lLeftDown, oNewMenu, nNewLevel, ;
	 nEvent, oMenuItem, nMenuItem, nReturn, nCol, nRow, oMsg, ;
	 nTemp, bKeyBlock, lSubMenu, aMode, aSavMenuSys

   if oStatus==NIL
      menuSysInit()
   endif

   aSavMenuSys := map()
   for k in oStatus KEYS
	aSavMenuSys[k] := oStatus[k]
   next


   oMsg := map()
   oMsg:flag := NIL
   oMsg:row  := nMsgRow
   oMsg:left := nMsgLeft
   oMsg:right:= nMsgRight
   oMsg:color:= iif(cMsgColor==NIL, setcolor(), cMsgColor)
   oMsg:back1:= NIL
   oMsg:back2:= NIL
   oMsg:fore := NIL
   oMsg:fontcol:= NIL
   oMsg:fontrow:= NIL

   nReturn     := 0


   IF ( oMsg:flag := ( ValType( oMsg:row ) + ;
			     ValType( oMsg:left ) + ;
			     ValType( oMsg:right ) == "NNN" ) )

      IF !( ValType( oMsg:color ) == "C" )
	 oMsg:color := GetClrPair( SetColor(), 1 )
      ENDIF

      @ oMsg:row, oMsg:left CLEAR TO ;
	oMsg:row, oMsg:right

      oStatus:scrVmsg := SaveScreen( oMsg:row, ;
	   oMsg:left, oMsg:row, oMsg:right )

   ENDIF

   oTopMenu:Select( nSelection )

   IF ( oTopMenu:ClassName != "TOPBARMENU" .AND. !oTopMenu:IsOpen )
      oTopMenu:Open()
   ELSE
      oTopMenu:Display()
   ENDIF

   IF nSelection <= 0
      WHILE ( nSelection <= 0 )

	 nEvent := SET( _SET_EVENTMASK, INKEY_KEYBOARD + INKEY_LDOWN )
	 nKey   :=  Inkey( 0.01 )
	 SET( _SET_EVENTMASK, nEvent )

	 IF ( nKey == K_LBUTTONDOWN .OR. nKey == K_LDBLCLK )
	    nSelection := oTopMenu:HitTest( MRow(), MCol() )

	 ELSEIF ( ( nSelection := oTopMenu:GetAccel( nKey ) ) != 0 )

	 ELSEIF ( IsShortCut( oTopMenu, nKey, @nReturn ) )
	    for k in aSavMenuSys KEYS
		oStatus[k] := aSavMenuSys[k]
	    next
	    RETURN ( nReturn )

	 ELSE
	    nSelection := 1

	 ENDIF

      ENDDO

      oTopMenu:Select( nSelection )
      oTopMenu:Display()

   ENDIF

   IF ( !oTopMenu:GetItem( nSelection ):Enabled .or. ;
	oTopMenu:GetItem( nSelection ):Disabled )
	    for k in aSavMenuSys KEYS
		oStatus[k] := aSavMenuSys[k]
	    next
      RETURN ( 0 )
   ENDIF

   oStatus:oMenu          := oTopMenu
   oStatus:menuList      := Array( 16 )
   oStatus:menuLevel     := 1
   oStatus:menuList[ 1 ] := oStatus:oMenu

   lLeftDown := mLeftDown()

   ShowMsg( oMsg, .T. )

   WHILE (.T.)
      nKey := INKEY( 0 )
      IF !( ( bKeyBlock := SETKEY( nKey ) ) == NIL )
	 EVAL( bKeyBlock, PROCNAME(1), PROCLINE(1), "" )
	 LOOP
      ENDIF

      IF ( nKey == K_MOUSEMOVE )

	 IF ( lLeftDown )
	    IF ( !MHitTest( @oNewMenu, @nNewLevel, @nNewItem, oTopMenu ) )

	    ELSEIF ( nNewLevel != oStatus:menuLevel )

	       IF ( nNewItem == oNewMenu:Current )
	       ELSEIF ( oNewMenu:GetItem( nNewItem ):Enabled .and. ;
			!oNewMenu:GetItem( nNewItem ):Disabled)
		  oStatus:oMenu := oNewMenu
		  PopChild( nNewLevel, oTopMenu )
		  oStatus:oMenu:Select( nNewItem )
		  oStatus:oMenu:Display()
		  PushMenu( .T., oTopMenu )
		  ShowMsg( oMsg, .T. )
	       ENDIF

	    ELSEIF ( nNewItem != oNewMenu:Current() )

	       PopChild( oStatus:menuLevel, oTopMenu  )

	       IF ( oStatus:oMenu:GetItem( nNewItem ):Enabled .and. ;
		    !oStatus:oMenu:GetItem( nNewItem ):Disabled )
		  oStatus:oMenu:Select( nNewItem )
		  oStatus:oMenu:Display()
		  PushMenu( .T., oTopMenu )
		  ShowMsg( oMsg, .T. )
	       ENDIF

	    ENDIF

	 ENDIF

      ELSEIF ( nKey == K_DOWN )

	 IF oStatus:oMenu:ClassName == "TOPBARMENU"
	    IF ( PushMenu( .T., oTopMenu ) )
	       ShowMsg( oMsg, .T. )
	    ENDIF
	 ELSE
	    nTemp := oStatus:oMenu:GetNext()
	    IF ( nTemp == 0 )
	       nTemp := oStatus:oMenu:GetFirst()
	    ENDIF
	    oStatus:oMenu:Select( nTemp )
	    oStatus:oMenu:Display()
	    ShowMsg( oMsg, .T. )

	 ENDIF

      ELSEIF ( nKey == K_UP )

	 IF oStatus:oMenu:ClassName != "TOPBARMENU"
	    nTemp := oStatus:oMenu:GetPrev()
	    IF ( nTemp == 0 )
	       nTemp := oStatus:oMenu:GetLast()
	    ENDIF
	    oStatus:oMenu:Select( nTemp )
	    oStatus:oMenu:Display()
	    ShowMsg( oMsg, .T. )

	 ENDIF

      ELSEIF ( nKey == K_LEFT )
	 IF ( lSubMenu := oStatus:menuLevel > 1 )
	    PopMenu( oTopMenu )
	 ENDIF
	 IF oStatus:oMenu:ClassName == "TOPBARMENU"
	    nTemp := oStatus:oMenu:GetPrev()
	    IF ( nTemp == 0 )
	      nTemp := oStatus:oMenu:GetLast()
	    ENDIF
	    oStatus:oMenu:Select( nTemp )
	    oStatus:oMenu:Display()
	    IF lSubMenu
	       PushMenu( .T., oTopMenu )
	    ENDIF
	 ENDIF
	 ShowMsg( oMsg, .T. )

      ELSEIF ( nKey == K_RIGHT )
	 IF ( lSubMenu := oStatus:menuLevel > 1 )
	    IF !( PopMenu( oTopMenu ) )
	       PopAll()
	    ENDIF
	    PopMenu( oTopMenu )
	 ENDIF

	 IF oStatus:oMenu:ClassName == "TOPBARMENU"
	    nTemp := oStatus:oMenu:GetNext()
	    IF ( nTemp == 0 )
	       nTemp := oStatus:oMenu:GetFirst()
	    ENDIF
	    oStatus:oMenu:Select( nTemp )
	    oStatus:oMenu:Display()
	    IF lSubMenu
	       PushMenu( .T., oTopMenu )
	    ENDIF
	 ENDIF
	 ShowMsg( oMsg, .T. )

      ELSEIF ( nKey == K_ENTER )
	 IF ( PushMenu( .T., oTopMenu ) )
	    ShowMsg( oMsg, .T. )
	 ELSE
	    ShowMsg( oMsg, .F. )
	    nReturn := Execute( oTopMenu )
	    IF ( nReturn != 0 )
	       EXIT
	    ENDIF

	 ENDIF

      ELSEIF ( nKey == K_ESC )
	 IF PopMenu( oTopMenu )
	    oStatus:oMenu:Display()
	    ShowMsg( oMsg, .T. )
	 ELSE

	    IF oStatus:oMenu:ClassName == "POPUPMENU"
	       oStatus:oMenu:Close()
	    ENDIF
	    nReturn := -1
	    EXIT

	 ENDIF

      ELSEIF ( nKey == K_LBUTTONDOWN )
	 IF ( !MHitTest( @oNewMenu, @nNewLevel, @nNewItem, oTopMenu ) )
	    IF ( !( GetList == NIL ) .AND. ;
	       HitTest( GetList, MROW(), MCOL(), oMsg ) != 0 )
	       GETACTIVE():ExitState := GE_MOUSEHIT
	       //ReadStats( SNLASTEXIT, GE_MOUSEHIT )
	       IF oStatus:oMenu:ClassName == "POPUPMENU"
		  PopMenu( oTopMenu )
	       ENDIF
	       nReturn := -1
	       EXIT
	    ENDIF

	    IF oStatus:oMenu:ClassName == "POPUPMENU"
	       PopMenu( oTopMenu )
	    ENDIF

	 ELSEIF ( nNewLevel == oStatus:menuLevel )
	    oStatus:oMenu:Select( nNewItem )
	    oStatus:oMenu:Display()
	    PushMenu(.T., oTopMenu )
	    ShowMsg( oMsg, .T. )

	 ELSE
	    oStatus:menuLevel := nNewLevel
	    oStatus:oMenu      := ;
	    oStatus:menuList[ oStatus:menuLevel ]

	    nMenuItem := oStatus:oMenu:Current
	    oMenuItem := oStatus:oMenu:GetItem( nMenuItem )
	    IF ( ( oMenuItem := ;
	       oStatus:oMenu:GetItem( ;
	       oStatus:oMenu:Current ) ):IsPopUp() )
	       oMenuItem:Data:Close()
	    ENDIF

	    IF !( nMenuItem == nNewItem )
	       nMenuItem    := nNewItem
	       oStatus:oMenu:Select( nNewItem )
	       oStatus:oMenu:Display()
	       PushMenu( .T., oTopMenu )
	    ENDIF

	    ShowMsg( oMsg, .T. )
	 ENDIF

	 lLeftDown := .T.

      ELSEIF ( nKey == K_LBUTTONUP )
	 lLeftDown := .F.
	 IF ( !MHitTest( @oNewMenu, @nNewLevel, @nNewItem, oTopMenu ) )
	 ELSEIF ( nNewLevel == oStatus:menuLevel )
	    IF ( nNewItem == oStatus:oMenu:Current )
	       ShowMsg( oMsg, .F.)
	       nReturn := Execute( oTopMenu )
	       IF ( nReturn != 0 )
		  EXIT
	       ENDIF
	    ENDIF

	 ENDIF

      ELSEIF ( ( nNewItem := oStatus:oMenu:GetAccel( nKey ) ) != 0 )
	 IF oStatus:oMenu:GetItem( nNewItem ):Enabled .and. ;
	    !oStatus:oMenu:GetItem( nNewItem ):Disabled

	    oStatus:oMenu:Select( nNewItem )
	    oStatus:oMenu:Display()

	    IF ( !PushMenu( .T., oTopMenu ) )
	       ShowMsg( oMsg, .F. )
	       nReturn := Execute( oTopMenu )
	       IF ( nReturn != 0 )
		  EXIT
	       ENDIF
	    ENDIF
	    ShowMsg( oMsg, .T. )

	 ENDIF

      ELSEIF ( IsShortCut( oTopMenu, nKey, @nReturn ) )

	 IF ( nReturn != 0 )
	    EXIT
	 ENDIF

      ELSEIF ( !( GetList == NIL ) .AND. ;
	 ( ( nNewItem := Accelerator( GetList, nKey, oMsg ) ) != 0 ) )
	 GETACTIVE():ExitState := GE_SHORTCUT
	 //ReadStats( SNNEXTGET, nNewItem )
	 IF oStatus:oMenu:ClassName == "POPUPMENU"
	    PopMenu( oTopMenu )
	 ENDIF
	 nReturn := -1
	 EXIT

      ELSEIF ( ( nNewItem := oTopMenu:GetAccel( nKey ) ) != 0 )
	 IF oTopMenu:GetItem( nNewItem ):Enabled .and. ;
	    !oTopMenu:GetItem( nNewItem ):Disabled

	    PopAll( oTopMenu )
	    oStatus:oMenu:Select( nNewItem )
	    oStatus:oMenu:Display()
	    IF ( oTopMenu:GetItem( nNewItem ):IsPopUp() )
	       PushMenu( .T., oTopMenu )
	    ELSE
	       ShowMsg( oMsg, .F. )
	       nReturn := Execute( oTopMenu )
	       IF ( nReturn != 0 )
		  EXIT
	       ENDIF
	    ENDIF
	    ShowMsg( oMsg, .T. )
	 ENDIF

      ENDIF

   ENDDO

   IF ( oMsg:flag )
      RestScreen( oMsg:row, oMsg:left, oMsg:row, ;
		  oMsg:right, oStatus:scrVmsg )
   ENDIF

   PopAll( oTopMenu )
   SetPos( nRow, nCol )
   SetCursor( lSaveCursor )

   for k in aSavMenuSys KEYS
	   oStatus[k] := aSavMenuSys[k]
   next

RETURN ( nReturn )
