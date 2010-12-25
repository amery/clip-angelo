static function tf_DelBytes( nLen )
     LOCAL cTmpFile, cBuffer
     LOCAL nHandle, nRecno, nCopy, nRead
     IF nLen == NIL .OR. empty(nLen)
          RETURN .F.
     ENDIF
     cTmpFile := NoExt(::cName)+".#$#"
     cBuffer  := space(::nBufferLen)
     nHandle  := Fcreate(cTmpFile)
     nRecno   := Fseek( ::hFile, 0, FS_RELATIVE )
     nCopy    := 0
     IF nHandle < 0
          RETURN .F.
     ENDIF
     Fseek( ::hFile, 0, FS_SET )
     DO WHILE nRecno > 0 .AND. Fseek( ::hFile, 0, FS_RELATIVE ) < nRecno
          nRead := FRead( ::hFile, @cBuffer, ::nBufferLen )
          nCopy := Min(nRead, nRecno - FSeek( nHandle, 0, FS_RELATIVE ) )
          FWrite(nHandle, cBuffer, nCopy)
     ENDDO
     Fseek( ::hFile, nRecno + nLen, FS_SET )
     nRead := ::nBufferLen
     DO WHILE nRead == ::nBufferLen
          nRead := FRead( ::hFile, @cBuffer, ::nBufferLen )
          FWrite(nHandle, cBuffer, nRead)
     ENDDO
     Fclose(nHandle)
     Fclose(::hFile)
     Ferase(::cName)
     Frename(cTmpFile, ::cName)
     ::nMode := FO_READWRITE + FO_EXCLUSIVE
     ::hFile = FOpen( ::cName, ::nMode )
     nRecno++
     IF nRecno >= ::nLen
          ::nLen := Fseek( ::hFile, 0, FS_END )
     ELSE
          Fseek( ::hFile, nRecno+1, FS_SET )
     ENDIF
     ::nLen -= nLen
RETURN .T.
