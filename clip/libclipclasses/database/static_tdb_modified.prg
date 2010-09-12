static function tdb_Modified()
   local i, cField
   for i := 1 to Len( ::aFldNames )
      cField = ( ::cAlias )->( FieldName( i ) )
      if ( ::cAlias )->( FieldGet( i ) ) != ::aBuffer[ i ]
         return .t.
      endif
   next
return .f.
