FUNCTION ReadUpdated( lUpdated )
   LOCAL lSavUpdated := oStatus:updated
   IF ( PCOUNT() > 0 )
      oStatus:updated := lUpdated
   ENDIF
RETURN ( lSavUpdated )
