static function HTTP_ErrorBlock(self,err)
	local i,s:="",data:="", crlf:=chr(13)+chr(10)
	i := 1
	s+="Run Time Error in HTTP-service:"+crlf
	while ( !Empty(ProcName(i)) )
		s += "Called from "+allTrim(ProcName(i)) + ;
			"(" + str(ProcLine(i)) + ")"+crlf

		i++
	end
	s += errorMessage(err)
	outlog(s)
	self:sendError(HTTP_ERR_INTERNALSERVERERROR,s)
return
