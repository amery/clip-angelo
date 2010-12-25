STATIC FUNCTION PushMenu( lSelect, oTopMenu )
   LOCAL oNewMenu

   oNewMenu := oStatus:oMenu:GetItem( oStatus:oMenu:Current )

   IF ( !( ValType( oNewMenu ) == "O" ) )
   ELSEIF ( oNewMenu:IsPopUp() )

      IF ( !( ValType( lSelect ) == "L" ) )
	 lSelect := .F.
      ENDIF

      oStatus:oMenu := oNewMenu:Data
      oStatus:menuList[ ++oStatus:menuLevel ] := ;
				 oStatus:oMenu

      IF ( lSelect )
	 oStatus:oMenu:Select( oStatus:oMenu:GetFirst() )
      ELSE
	 oStatus:oMenu:Select( 0 )
      ENDIF

      IF !( oStatus:oMenu:IsOpen )
	 oStatus:oMenu:Open()
      ENDIF

      RETURN ( .T. )

   ENDIF

RETURN ( .F. )
