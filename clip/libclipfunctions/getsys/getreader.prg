PROCEDURE GetReader( oGet, GetList, oMenu, oMsg )
   LOCAL nKey, nRow, nCol, nCursor

   IF ( oStatus:lastexit == GE_SHORTCUT .OR. ;
	oStatus:lastexit == GE_MOUSEHIT .OR. ;
	GetPreValidate( oGet, oMsg ) )

      ShowGetMsg( oGet, oMsg )

      oStatus:hitcode  := 0
      oGet:setFocus()

      WHILE ( oGet:exitState == GE_NOEXIT .AND. !oStatus:killread )

#ifdef __________
	 IF ( oGet:typeOut )
	    oGet:exitState := GE_ENTER
	 endif
#else
	 IF ( oGet:typeOut )
	    //  oGet:exitState := GE_ENTER

	    IF oStatus:Pos = NIL
	      oGet:exitState := GE_ENTER
	    ELSE
	      IF oStatus:LastExit = GE_UP
		IF oStatus:Pos-1 == 0
		  oGet:exitState := GE_ENTER
		ELSE
		  oGet:exitState := GE_UP
		ENDIF
	      ELSE
		oGet:exitState := GE_ENTER
	      ENDIF

	    ENDIF

	 ENDIF
#endif
	 WHILE ( oGet:exitState == GE_NOEXIT .AND. !oStatus:killread )
	    SETCURSOR( IIF( oStatus:svccursor == SC_NONE, ;
		 SC_NORMAL, oStatus:svccursor ) )
	    nKey := INKEY( 0 )
	    SETCURSOR( SC_NONE )
	    GetApplyKey( oGet, nKey, GetList, oMenu, oMsg )
	    nRow := ROW()
	    nCol := COL()
	    ShowGetMsg( oGet, oMsg )
	    DevPos( nRow, nCol )
	 ENDDO

	 IF oStatus:lastexit == GE_SHORTCUT
	 ELSEIF oStatus:lastexit == GE_MOUSEHIT
	 ELSEIF ( !GetPostValidate( oGet, oMsg ) )
	    oGet:exitState := GE_NOEXIT
	 ENDIF

      ENDDO
      oStatus:lastexit := 0

      nRow    := ROW()
      nCol    := COL()
      nCursor := SETCURSOR()
      oGet:killFocus()
      SETCURSOR( nCursor )
      SETPOS( nRow, nCol )

      EraseGetMsg( oGet, oMsg )
   ENDIF
RETURN
