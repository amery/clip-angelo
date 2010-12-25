FUNCTION INPUTMODE(lDummy)
  LOCAL i := 1

  DO WHILE .NOT. EMPTY(PROCNAME(i))
    DO CASE
      CASE PROCNAME(i) == "READMODAL"
        DO CASE
          CASE PROCNAME(i +1) == "GETINPUT"
            RETURN( IM_GETINPUT )
          CASE PROCNAME(i +1) == "GETSECRET"
            RETURN( IM_GETSECRET )
          OTHERWISE
            RETURN( IM_READ )               // READ
        ENDCASE
      CASE PROCNAME(i) == "__ACCEPT"
        RETURN( IM_ACCEPT )                 // ACCEPT, INPUT (3)
      CASE PROCNAME(i) == "__WAIT"
        RETURN( IM_WAIT )                   // WAIT
      CASE PROCNAME(i) == "__MENUTO"
        RETURN( IM_MENU )                   // MENU
      CASE PROCNAME(i) == "INKEYTRAP"
        RETURN( IM_INKEY )                  // INKEY
      CASE PROCNAME(i) == "MEMOEDIT"
        RETURN( IM_MEMOEDIT )               // MEMOEDIT
      CASE PROCNAME(i) == "ACHOICE"
        RETURN( IM_ACHOICE )                // ACHOICE
    ENDCASE
    i++
  ENDDO
RETURN( 0 )
