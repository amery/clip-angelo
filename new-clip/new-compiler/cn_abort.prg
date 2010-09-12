

Procedure cn_Abort ()
	Local ln01ii := 0
	Local ln02ii := 0
	For ln01ii := 1 To Len( p_aClipError)
		For ln02ii := 1 To Len( p_aClipError [ ln01ii] )
			OutErr ( aClipError [ ln01ii, ln02ii] )
		Next
	Next


