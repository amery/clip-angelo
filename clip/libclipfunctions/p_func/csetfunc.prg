FUNCTION CSETFUNC(nKey, cVal)
  LOCAL cResult := ""

  IF nKey >= 1 .AND. nKey <= MAX_FUNC
    IF cVal = NIL .AND. _aFuncTable[nKey] <> NIL
      cResult := _aFuncTable[nKey]
    ELSE
      _aFuncTable[nKey] := cVal
    ENDIF
  ENDIF

RETURN( cResult )
