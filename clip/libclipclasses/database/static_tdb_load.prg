static func tdb_load()
   local n
   if ::lBuffer
      if Empty( ::aBuffer )
         ::aBuffer = Array( ::FCount() )
      endif
      for n = 1 to Len( ::aBuffer )
         ::aBuffer[ n ] = ( ::nArea )->( FieldGet( n ) )
      next
      if ::lOemAnsi
         ::OemToAnsi()
      endif
   endif
return nil
