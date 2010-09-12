static function tdb_Create()
   local oField, i, aStruct:= Array( Len( ::aFields ) )
   for i:= 1 to len( ::aFields )
       oField:= ::aFields[ i ]
       aStruct[i] := { oField:cName, oField:cType, oField:nLen, oField:nDec }
   next
   ::DbCreate( aStruct )
return nil
