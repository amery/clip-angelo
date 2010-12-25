static function tf_GetByte()
       local cByte := " "
return iif( FRead( ::hFile, @cByte, 1 ) == 1, Asc( cByte ), -1 )
