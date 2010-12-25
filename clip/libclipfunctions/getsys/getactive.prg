FUNCTION GetActive( g )
	LOCAL oldActive := oStatus:activeget
	IF ( PCOUNT() > 0 )
		oStatus:activeget := g
	ENDIF
RETURN ( oldActive )
