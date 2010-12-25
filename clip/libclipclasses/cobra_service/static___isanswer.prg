static function __isAnswer(oAnswer)
	if valtype(oAnswer) != "O"
		return .f.
	endif
	if !(oAnswer:className == "COBRAANSWER")
		return .f.
	endif
return .t.
