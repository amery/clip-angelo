FUNCTION IsShortCut( oMenu, nKey, nID )
   LOCAL nItem, nTotal, nShortCut, oItem, i

   IF !( oMenu:ClassName == "TOPBARMENU" )
      RETURN ( IsQuick( oMenu, nKey, @nID ) )

   ELSEIF ( ( nShortCut := oMenu:GetShortCt( nKey ) ) > 0 ) .AND. ;
	    ( ( oItem := oMenu:GetItem( nShortcut ) ):Enabled .and. ;
		!oItem:Disabled ) .AND. ;
	    ( !( oItem:IsPopUp() ) )
      oMenu:Select( nShortCut )
      EVAL( oItem:Data, oItem )
      nID := oItem:ID
      RETURN ( .T. )

   ELSEIF ( nShortCut == 0 )
      nTotal := oMenu:ItemCount()
      nItem  := oMenu:Current
      nItem  := IIF( nItem == 0, nItem := 1, nItem )

      FOR i := 1 TO nTotal
	 IF ( !( oItem := oMenu:GetItem( nItem ) ):Enabled .or. oItem:Disabled)
	 ELSEIF ( !oItem:IsPopUp() )
	 ELSEIF ( IsQuick( oItem:Data, nKey, @nID ) )
	    RETURN ( .T. )
	 ENDIF
	 nItem := IIF( ++nItem > nTotal, nItem := 1, nItem )
      NEXT

   ENDIF
RETURN ( .F. )
