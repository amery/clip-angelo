PROCEDURE GUIReader( oGet, GetList, oMenu, oMsg )
   LOCAL oGUI, nKey := 0, nRow, nCol

   IF ( VALTYPE( oGet:Control ) == "O" ) .AND. ;
	oStatus:lastexit == GE_SHORTCUT .OR. ;
	oStatus:lastexit == GE_MOUSEHIT .OR. ;
	GetPreValidate( oGet, oMsg )

      ShowGetMsg( oGet, oMsg )
      oStatus:lastexit := 0

      oGUI := oGet:Control
      oGUI:Select( oGet:VarGet() )
      oGUI:setFocus()

      IF oGet:exitState == GE_NOEXIT

	 IF ( oStatus:hitcode > 0 )
	    oGUI:Select( oStatus:hitcode )

	 ELSEIF ( oStatus:hitcode == HTCAPTION )
	    oGUI:Select()

	 ELSEIF ( oStatus:hitcode == HTCLIENT )
	    oGUI:Select( K_LBUTTONDOWN )

	 ELSEIF ( oStatus:hitcode == HTDROPBUTTON )
	    oGUI:Open()

	 ELSEIF ( ( oStatus:hitcode >= HTSCROLLFIRST ) .AND. ;
		  ( oStatus:hitcode <= HTSCROLLLAST ) )
	    oGUI:Scroll( oStatus:hitcode )
	 ENDIF

      ENDIF

      oStatus:hitcode := 0

      WHILE ( oGet:exitState == GE_NOEXIT .AND. !oStatus:killread )

	 IF ( oGUI:typeOut )
	    oGet:exitState := GE_ENTER
	 ENDIF

	 WHILE ( oGet:exitState == GE_NOEXIT .AND. !oStatus:killread )
	    GUIApplyKey( oGet, oGUI, GetList, INKEY(0), oMenu, oMsg )
	    ShowGetMsg( oGet, oMsg )
	    DevPos( nRow, nCol )
	 ENDDO

	 IF     oStatus:lastexit  == GE_SHORTCUT
	 ELSEIF oStatus:lastexit  == GE_MOUSEHIT
	 ELSEIF ( !GetPostValidate( oGet, oMsg ) )
	    oGet:exitState := GE_NOEXIT
	 ENDIF

      ENDDO

      IF ( ( oGUI:ClassName() $ "LISTBOX_RADIOGROUP" ) .AND. ;
	 VALTYPE( oGet:VarGet() ) == "N" )
	 oGet:VarPut( oGUI:Value )
      ELSE
	 oGet:VarPut( oGUI:Buffer )
      ENDIF
      oGUI:killFocus()

      EraseGetMsg( oGet, oMsg )

      IF !( oGUI:ClassName() == "LISTBOX" )
      ELSEIF ( !oGUI:DropDown )
      ELSEIF ( oGUI:IsOpen )
	 oGUI:Close()
      ENDIF
   ENDIF
RETURN
