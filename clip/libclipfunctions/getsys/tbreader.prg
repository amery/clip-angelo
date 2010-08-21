PROCEDURE TBReader( oGet, GetList, oMenu, oMsg )
   LOCAL oTB, nKey, lAutoLite, nCell, nSaveCursor, nProcessed
   LOCAL nRow, nCol

   IF ( VALTYPE( oGet:Control ) == "O" ) .AND. ;
	oStatus:lastexit == GE_SHORTCUT .OR. ;
	oStatus:lastexit == GE_MOUSEHIT .OR. ;
	GetPreValidate( oGet, oMsg )

      ShowGetMsg( oGet, oMsg )
      oStatus:lastexit := 0

      nSaveCursor := SetCursor( SC_NONE )

      oTB := oGet:Control

      lAutoLite := oTB:Autolite
      oTB:Autolite := .T.
      oTB:Hilite()

      IF oGet:exitState == GE_NOEXIT
	 IF ( oStatus:hitcode == HTCELL )
	    oTB:RowPos := oTb:mRowPos
	    oTB:ColPos := oTb:mColPos
	    oTB:Invalidate()
	 ENDIF
      ENDIF

      oStatus:hitcode := 0

      WHILE ( oGet:exitState == GE_NOEXIT .AND. !oStatus:killread )

	 WHILE ( oGet:exitState == GE_NOEXIT .AND. !oStatus:killread )
	    nKey := 0

	    WHILE ( !( oTB:Stabilize() ) .AND. ( nKey == 0 ) )
	       nKey := INKEY()
	    ENDDO

	    IF ( nKey == 0 )
	       nKey := INKEY(0)
	    ENDIF

	    nProcessed := oTB:ApplyKey( nKey )
	    IF ( nProcessed == TBR_EXIT )
	       oGet:exitState := GE_ESCAPE
	       EXIT

	    ELSEIF ( nProcessed == TBR_EXCEPTION )
	       TBApplyKey( oGet, oTB, GetList, nKey, oMenu, oMsg )
	       ShowGetMsg( oGet, oMsg )
	    ENDIF

	 ENDDO

	 IF     oStatus:lastexit  == GE_SHORTCUT
	 ELSEIF oStatus:lastexit  == GE_MOUSEHIT
	 ELSEIF ( !GetPostValidate( oGet, oMsg ) )
	    oGet:exitState := GE_NOEXIT
	 ENDIF

      ENDDO

      oTB:Autolite := lAutoLite
      oTB:DeHilite()
      EraseGetMsg( oGet, oMsg )
      SetCursor( nSaveCursor )
   ENDIF
RETURN
