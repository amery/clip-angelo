FUNCTION IsQuick( oMenu, nKey, nID )
   LOCAL nItem, nTotal, nShortCut, oItem, i

   IF ( ( nShortCut := oMenu:GetShortCt( nKey ) ) == 0 )
      nTotal := oMenu:ItemCount()

      FOR nItem := 1 TO nTotal
	 IF ( !( oItem := oMenu:GetItem( nItem ) ):Enabled .or. oItem:Disabled )
	 ELSEIF ( !( oItem:IsPopUp() ) )
	 ELSEIF ( IsQuick( oItem:Data, nKey, @nID ) )
	    RETURN ( .T. )
	 ENDIF
      NEXT

   ELSEIF ( !( oItem := oMenu:GetItem( nShortCut ) ):IsPopUp() )
      IF oItem:Enabled .and. !oItem:Disabled
	 oMenu:Select( nShortCut )
	 EVAL( oItem:Data, oItem )
	 nID := oItem:ID
	 RETURN ( .T. )
      ENDIF

   ENDIF

RETURN ( .F. )
