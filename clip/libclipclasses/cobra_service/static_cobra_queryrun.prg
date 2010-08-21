static function COBRA_queryRun(self,oQuery,oIniData)
	local oAnswer := cobraAnswerNew()
	local e,err,oErr, alias

	self:errno := 0
	self:error := ""

	if !__isQuery(oQuery)
		self:errno := COBRA_ERR_BADQUERY
		return oAnswer
	endif
	err := errorBlock({|e|break(e)})

	begin sequence
	oAnswer:id := oQuery:id
	do case
		case oQuery:command == "CODBUNIFY"
			__run_codbunify(oQuery:args,oAnswer)
		case oQuery:command == "CODBSELECT"
			__run_codbselect(oQuery:args,oAnswer)
		case oQuery:command == "CODBDELOBJ"
			__run_codbDelObj(oQuery:args,oAnswer)
		case oQuery:command == "CODBGETOBJ"
			__run_codbGetObj(oQuery:args,oAnswer)
		case oQuery:command == "CODBGETNOBJ"
			__run_codbGetNObj(oQuery:args,oAnswer)
		case oQuery:command == "CODBMETABYNAME"
			__run_codbmetabyname(oQuery:args,oAnswer)
		case oQuery:command == "CODBUPDOBJ"
			__run_codbUpdObj(oQuery:args,oAnswer)
		case oQuery:command == "CODBADDOBJ"
			__run_codbAddObj(oQuery:args,oAnswer)
		case oQuery:command == "CLNT"
			__check_clnt_info(self:info,oQuery:args,oAnswer)
		case oQuery:command == "AUTH"
			__run_auth(self:info,oQuery:args,oAnswer,oIniData)
		case oQuery:command == "GETFILE"
			__run_getfile(oQuery:args,oAnswer)
		case oQuery:command == "GETMYFILE"
			__run_getmyfile(self,oQuery:args,oAnswer)
		otherwise
			if valType(oIniData) == "O" .and. oIniData:className=="INIFILE"
				alias := oIniData:getValue("ALIASES",oQuery:command)
			endif
			if empty(alias)
				oAnswer:errno := COBRA_ERR_BADCOMMAND
			else
				//oAnswer:return := clipA(alias,oQuery:args)
				aSize(oQuery:args,len(oQuery:args)+3)
				ains(oQuery:args,1)
				oQuery:args[1] := oAnswer
				ains(oQuery:args,1)
				oQuery:args[1] := oIniData
				ains(oQuery:args,1)
				oQuery:args[1] := self
				clipA(alias,oQuery:args)
			endif
	end
	recover using oErr
		oAnswer:errno := COBRA_ERR_RUNTIME
		oAnswer:error := errorMessage(oErr)
	end sequence
	errorBlock(err)
	//outlog(__FILE__,__LINE__,oAnswer)
return oAnswer
