static function COBRA_answerWait(nQuery)
	local ret:=.t.,e,err,oErr
	local i,cAnswer,oAnswer,num,data
	local oQuery, oAnswerQuery
	if valtype(nQuery) != "N"
		ret := .f.
	endif
	if ret .and. ! (nQuery $ ::__queries)
		ret := .f.
	endif
	if ret
		oQuery := ::__queries[nQuery]
	endif
	if !ret .or. !__isQuery(oQuery)
		::errno := COBRA_ERR_BADQUERY
		::error := [Bad cobraQuery object]
		return .f.
	endif
	oQuery := NIL
	while .t.
		if !::receiveLine()
			ret := .f.
			exit
		endif
		i := at(" ",::cLine)
		if i<=0
			i := len(::cLine)+1
		endif
		cAnswer := left(::cLine,i-1)
		::cLine := substr(::cLine,i+1)
		if cAnswer == "ERROR"
			::error := ::cLine
			::cLine := ""
			ret := .f.
			exit
		endif
		num := val(::cLine)
		i := at(" ",::cLine)
		if i<=0
			i := len(::cLine)+1
		endif
		data := substr(::cLine,i+1)
		if cAnswer == "ANSWER"
			::cLine := ""
			if num $ ::__queries
				oAnswerQuery := ::__queries[nQuery]
				oAnswerQuery:answer := str2var(data)
			else
				::errno := COBRA_ERR_BADANSWER
				::error := [Answer ID not found]
				ret := .f.
			endif
			if num==nQuery
				exit
			endif
		endif
		/* �� ����� ��������� ������ - ������ ������ :) */
		if cAnswer == "QUERY"
			err := errorBlock({|e|break(e)})
			::cLine := ""
			oQuery := str2var(data)
			begin sequence
				oAnswer := ::queryRun(oQuery,::oIni)
				if !empty(::errno)
					oAnswer:id := num
					oAnswer:errno := ::errno
					oAnswer:error := ::error
				endif
			recover using oErr
				oAnswer := cobraAnswerNew()
				oAnswer:id := num
				oAnswer:errno := COBRA_ERR_RUNTIME
				oAnswer:error := errorMessage(oErr)
			end sequence
			errorBlock(err)
			if !::answerSend(oAnswer)
				oAnswer := cobraAnswerNew()
				oAnswer:id := num
				oAnswer:errno := ::errno
				oAnswer:error := ::error
				::answerSend(oAnswer)
			endif
		endif
	end
return ret
