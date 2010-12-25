static function tdb_save(self)
   local n
   if self:lBuffer
      if ! ( self:nArea )->( EoF() )
         if self:lShared
            if self:RecLock()
               for n := 1 to Len( self:aBuffer )
                  if self:lOemAnsi .and. ValType( self:aBuffer[ n ] ) == "C"
                     ( self:nArea )->( FieldPut( n, AnsiToOem( self:aBuffer[ n ] ) ) )
                  else
                     ( self:nArea )->( FieldPut( n, self:aBuffer[ n ] ) )
                  endif
               next
               self:UnLock()
            else
               if ! Empty( self:bNetError )
                  return Eval( self:bNetError, Self )
               else
                  MsgAlert( "Record in use", "Please, retry" )
               endif
            endif
         else
            for n := 1 to Len( self:aBuffer )
               if self:lOemAnsi .and. ValType( self:aBuffer[ n ] ) == "C"
                  ( self:nArea )->( FieldPut( n, AnsiToOem( self:aBuffer[ n ] ) ) )
               else
                  ( self:nArea )->( FieldPut( n, self:aBuffer[ n ] ) )
               endif
            next
         endif
      endif
   endif
return nil
