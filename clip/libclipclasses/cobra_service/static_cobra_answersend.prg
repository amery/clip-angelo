static function COBRA_answerSend(oAnswer)
	local ret := .t., str:=""
	if !__isAnswer(oAnswer)
		::errno := COBRA_ERR_BADANSWER
		::error := [Bad cobraAnswer object]
		return .f.
	endif
	str+="ANSWER "+alltrim(str(oAnswer:id))+" "
	str+=var2Str(oAnswer)
return	::sendLine(str)
