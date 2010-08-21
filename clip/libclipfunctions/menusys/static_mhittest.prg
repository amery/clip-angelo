STATIC FUNCTION MHitTest( oNewMenu, nNewLevel, nNewItem, oTopMenu )

   FOR nNewLevel := oStatus:menuLevel TO 1 STEP -1
      oNewMenu   := oStatus:menuList[ nNewLevel ]
      nNewItem   := oNewMenu:HitTest( mRow(), mCol() )
      IF ( nNewItem < 0 )
	 RETURN ( .F. )

      ELSEIF ( nNewItem > 0 ) .AND. ;
		oNewMenu:GetItem( nNewItem ):Enabled .and. ;
		!oNewMenu:GetItem( nNewItem ):Disabled
	 RETURN ( .T. )
      ENDIF
   NEXT
RETURN ( .F. )
