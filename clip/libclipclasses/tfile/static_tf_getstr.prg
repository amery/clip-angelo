static function tf_GetStr( nLen )
   local cStr := Space(nLen)
   local nRead
   nRead := FRead( ::hFile, @cStr, nLen )
return Left(cStr, nRead)
