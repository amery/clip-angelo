static function tdb_fieldGet( Pos )
   if ::lBuffer
      return ::aBuffer[ Pos ]
   else
      return ( ::nArea )->( FieldGet( Pos ) )
   endif
return nil
