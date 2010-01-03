/***
*  PrintIt( <cString> )
*  Print a string, THEN send a CRLF
*/
STATIC PROCEDURE PrintIt( cString )

   IF cString == NIL
      cString := ""
   ELSE
#ifdef S87_COMPAT
     // prevents output of trailing space, also prevents wrapping of some
     // lines when sent to screen or 80-column printer. Comment out this
     // line for complete Summer 87 compatibility.
     //cString := Trim( cString )
#endif
   ENDIF

   QQOUT( cString )
   QOUT()

   RETURN
