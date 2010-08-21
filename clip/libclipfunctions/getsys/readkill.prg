FUNCTION ReadKill( lKill )
   LOCAL lSavKill
   lSavKill := oStatus:killread
   IF ( PCOUNT() > 0 )
      oStatus:killread := lKill
   ENDIF
RETURN ( lSavKill )
