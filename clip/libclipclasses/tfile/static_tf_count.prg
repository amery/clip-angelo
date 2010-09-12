static function tf_Count( cStr )
   local cBuffer
   local nCounter, nRecno, nAt
   cStr := iif(cStr==NIL, CRLF, cStr)
   IF ::nLen == 0
     	RETURN 0
   ENDIF
   nCounter := 0
   nRecno   := Fseek( ::hFile, 0, FS_RELATIVE )
   FSeek( ::hFile, 0, FS_SET )
   while ! ::lEof()
     cBuffer := ::cGetStr( ::nBufferLen )
     while ( nAt := At( cStr, cBuffer ) ) > 0
        nCounter++
        cBuffer := Substr( cBuffer, nAt + Len( cStr ) )
     end
   end
   Fseek( ::hFile, nRecno, FS_SET )
return nCounter
