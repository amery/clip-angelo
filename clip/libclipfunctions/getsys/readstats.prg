FUNCTION ReadStats( nStatic, xValue )
   LOCAL xSavStats, sStatic
   switch ( nStatic )
	case   1
		sStatic := "updated"
	case   2
		sStatic := "format"
	case   3
		sStatic := "killread"
	case   4
		sStatic := "bumptop"
	case   5
		sStatic := "bumpbot"
	case   6
		sStatic := "lastexit"
	case   7
		sStatic := "lastpos"
	case   8
		sStatic := "activeget"
	case   9
		sStatic := "readvar"
	case  10
		sStatic := "procname"
	case  11
		sStatic := "procline"
	case  12
		sStatic := "nextget"
	case  13
		sStatic := "hitcode"
	case  14
		sStatic := "pos"
	case  15
		sStatic := "scrvmsg"
	case  16
		sStatic := "menuid"
	case  17
		sStatic := "svccursor"
   end
   nStatic:=hashname(upper(sStatic))
   xSavStats := oStatus[nStatic]
   IF ( PCOUNT() > 1 )
      oStatus[nStatic] := xValue
   ENDIF
RETURN ( xSavStats )
