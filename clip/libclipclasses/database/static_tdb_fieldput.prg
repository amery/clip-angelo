static function tdb_fieldPut(self, pos, val )
   if self:lBuffer
      self:aBuffer[ pos ] := val
   else
      if self:lShared
         if self:RecLock()
            ( self:nArea )->( FieldPut( pos, val ) )
            self:UnLock()
         else
            if ! Empty( self:bNetError )
               return Eval( self:bNetError, Self )
            endif
         endif
      else
         ( self:nArea )->( FieldPut( pos, val ) )
      endif
   endif
return nil
