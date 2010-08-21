static function tdb_WriteFields()
   local n, o, nLen := len( ::aFields ), nOldArea:= Select()
   DbCreate( ::cFile + EXT_FLD, aSTRU_FIELD )
   USE (::cFile + EXT_FLD) NEW
   for n = 1 to nLen
       o:= ::aFields[n]
       APPEND BLANK
       FieldPut( 1, o:cName )
       FieldPut( 2, o:cType )
       FieldPut( 3, o:nLen )
       FieldPut( 4, o:nDec )
   next
   CLOSE
   Select ( nOldArea )
Return nil
