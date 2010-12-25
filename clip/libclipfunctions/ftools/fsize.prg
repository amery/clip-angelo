FUNCTION FSize( nHandle )
  LOCAL nCurrent, nLength
  nCurrent := FPOS( nHandle )
  nLength := FGOBOTTOM( nHandle )
  FSEEK( nHandle, nCurrent, FS_SET )
  RETURN ( nLength )
