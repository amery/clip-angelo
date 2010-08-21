

Procedure dv_Warning
	Local ln01ii
	For ln01ii := 0 to pCount()
		OutErr( Param(ln01ii) )
	next
	OutErr( Chr(13))

