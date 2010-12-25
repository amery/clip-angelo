static function tdb_skipper( nrec )
   local nSkip
   nRec := iif( nrec==NIL, 1, nrec)
   nSkip = ( ::nArea )->( DbSkipper( nRec ) )
   if ::lBuffer
      ::Load()
   endif
return nSkip
