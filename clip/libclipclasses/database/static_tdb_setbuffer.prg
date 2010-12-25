static function tdb_setBuffer( flag )
	flag:=iif( flag==NIL, .t., flag )
	::lBuffer = flag
   	if ::lBuffer
      		::Load()
   	else
      		::aBuffer := nil
   	endif
return ::lBuffer
