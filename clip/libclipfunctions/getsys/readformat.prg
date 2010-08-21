FUNCTION ReadFormat( b )
   LOCAL bSavFormat := oStatus:format
   IF ( PCOUNT() > 0 )
      oStatus:format := b
   ENDIF
RETURN ( bSavFormat )
