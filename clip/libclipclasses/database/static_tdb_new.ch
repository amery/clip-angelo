static function tdb_new(self, wa )
   local n
   wa := iif(wa==NIL, Select(), wa)
   self:nArea     = wa
   self:cAlias    = Alias( wa )
   self:cFile     = Alias( wa )
   self:cDriver   = ( Alias( wa ) )->( DbSetDriver() )
   self:lShared   = .t.
   self:lReadOnly = .f.
   self:lBuffer   = .t.
   self:lOemAnsi  = .f.
   self:Load()
   self:aFldNames = {}
   for n = 1 TO ( self:cAlias )->( FCount() )
      AAdd( self:aFldNames, ( self:cAlias )->( FieldName( n ) ) )
   next
return Self
