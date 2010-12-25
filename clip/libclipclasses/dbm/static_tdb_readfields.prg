static function tdb_ReadFields()
   local nOldArea := Select()
   USE (::cFile + EXT_FLD) NEW READONLY
   While ! Eof()
      ::AddField( TField():New( Upper( FieldGet( 1 ) ),;  // Name.
                                Upper( FieldGet( 2 ) ),;  // Type.
                                FieldGet( 3 ),;           // Len.
                                FieldGet( 4 ) ) )         // Dec.
      SKIP
   end
   CLOSE
   Select ( nOldArea )
return nil
