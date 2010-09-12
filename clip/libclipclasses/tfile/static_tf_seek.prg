static function tf_Seek( cStr, nDirection, nCase )
   local cBuffer
   local nRecno, nOffset, nAt, nBufferLen, nCounter
   nDirection := iif(nDirection==NIL, SEEK_FORWARD, nDirection)
   nCase      := iif(nCase==NIL, SEEK_NOCASE, nCase)
   nRecno     := Fseek( ::hFile, 0, FS_RELATIVE )
   nBufferLen := ::nBufferLen
   nCounter   := 0
   cBuffer    := space(nBufferLen)
   While len(cBuffer) == ::nBufferLen
     IF nDirection == SEEK_BACKWARD
          nRecno     :=Fseek( ::hFile, 0, FS_RELATIVE )
          nBufferLen := Min(nBufferLen, nRecno)
          Fseek( ::hFile, -nBufferLen, FS_RELATIVE )
     ENDIF
     nOffset := Fseek( ::hFile, 0, FS_RELATIVE )
     cBuffer := ::cGetStr(nBufferLen)
     Do Case
     Case nCase == SEEK_UPCASE
          cBuffer := Upper(cBuffer)
     Case nCase == SEEK_LOCASE
          cBuffer := Lower(cBuffer)
     EndCase
     If nDirection == SEEK_FORWARD
          nAt := At(cStr,cBuffer)
     Else
          nAt := Rat(cStr,cBuffer)
     Endif
     If nAt > 0
          Fseek( ::hFile, nOffset + nAt - 1 , FS_SET )
          retu .T.
     Endif
     IF nDirection == SEEK_FORWARD
          Fseek( ::hFile, -len(cStr), FS_RELATIVE )
     ELSE
          Fseek( ::hFile, len(cStr), FS_RELATIVE )
     ENDIF
   Enddo
   Fseek( ::hFile, nRecno, FS_SET )
return .F.
