static function tf_InsBytes( nLen )
     LOCAL cTmpFile, cBuffer
     LOCAL nHandle, nRecno, nCopy, nRead
     IF nLen == NIL .OR. empty(nLen)
          RETURN .F.
     ENDIF
     IF ::lEof()
          ::AppStr( Space(nLen) )
          RETURN .T.
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
     Fseek( ::hFile, nRecno, FS_SET )
     Fwrite(nHandle, space(nLen), nLen)
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
     ::hFile := FOpen( ::cName, ::nMode )
     Fseek( ::hFile, nRecno, FS_SET )
     ::nLen += nLen
RETURN .T.
