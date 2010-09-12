static function tdb_oem2ansi()
   local n
   for n = 1 to Len( ::aBuffer )
      if ValType( ::aBuffer[ n ] ) == "C"
         ::aBuffer[ n ] = OemToAnsi( ::aBuffer[ n ] )
      endif
   next
return nil
