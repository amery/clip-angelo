STATIC FUNCTION PopMenu( oTopMenu )
   IF ( oStatus:menuLevel > 1 )
      oStatus:oMenu:Select(0)
      oStatus:oMenu:Close(.T.)
      oStatus:oMenu := ;
	 oStatus:menuList[ --oStatus:menuLevel ]
      RETURN ( .T. )
   ENDIF
RETURN ( .F. )
