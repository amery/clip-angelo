static function tdb_seek( Exp, flag )
   local Found
   flag := iif( flag==NIL, Set( _SET_SOFTSEEK ), flag )
   Found = ( ::nArea )->( DbSeek( Exp, flag ) )
   if ::lBuffer
      ::Load()
   endif
return Found
