STATIC FUNCTION PopAll( oTopMenu )
   IF oStatus:menuList[2] != NIL
      oStatus:menuList[2]:Close()
   ENDIF
   oStatus:menuLevel := 1
   oStatus:oMenu      := oStatus:menuList[ 1 ]
RETURN ( .T. )
