

Procedure dv_Abort ()
	Local ln01ii := 0
	Local ln02ii := 0
	For ln01ii := 1 To Len( p_aDevilError)
		For ln02ii := 1 To Len( p_aDevilError [ ln01ii] )
			OutErr ( p_aDevilError [ ln01ii, ln02ii] )
		Next
	Next


