

Procedure cn_Warning
	Local ln01ii
	For ln01ii := 0 to pCount()
		OutErr( Param(n01ii) )
	next
	OutErr( Chr(13))

