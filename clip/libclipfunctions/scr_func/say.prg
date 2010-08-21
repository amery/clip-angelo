function Say( Row, Col, string, pict, colors)
	iif ( colors==NIL, colors:=setcolor(), )
	if pict==NIL
		DevPos( Row, Col ) ; DevOut( string , colors )
	else
		DevPos( Row, Col ) ; DevOutPict( string, pict , colors )
	endif
return .t.
