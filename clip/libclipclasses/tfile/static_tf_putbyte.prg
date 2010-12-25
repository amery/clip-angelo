static function tf_PutByte( nByte )
   local cByte := Chr( nByte )
return FWrite( ::hFile, @cByte, 1 )
