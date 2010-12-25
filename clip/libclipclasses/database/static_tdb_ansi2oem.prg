static function tdb_ansi2oem()
   local n
   for n = 1 to Len( ::aBuffer )
      if ValType( ::aBuffer[ n ] ) == "C"
         ::aBuffer[ n ] = AnsiToOem( ::aBuffer[ n ] )
      endif
   next
return nil
