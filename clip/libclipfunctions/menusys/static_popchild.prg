STATIC FUNCTION PopChild( nNewLevel, oTopMenu )
   LOCAL oOldMenuItem, nCurrent

   IF ( ( nCurrent := oStatus:oMenu:Current ) != 0 )
      oOldMenuItem := oStatus:oMenu:GetItem( nCurrent )
      IF ( oOldMenuItem:IsPopUp() )
	 oOldMenuItem:Data:Close()
	 oStatus:menuLevel := nNewLevel
	 RETURN ( .T. )
      ENDIF
   ENDIF
RETURN ( .F. )
