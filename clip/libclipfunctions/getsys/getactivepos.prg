FUNCTION GetActivePos( pos )
   LOCAL oldActive := oStatus:pos
   IF ( PCOUNT() > 0 )
      oStatus:pos := pos
   ENDIF
RETURN ( oldActive )
