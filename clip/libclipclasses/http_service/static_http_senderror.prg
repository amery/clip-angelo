static function HTTP_sendError(self,nError,data1,data2,data3)
	local crlf:=chr(13)+chr(10)
	local data:=crlf,ret_code:="",s1:="",s2:=""
	//outlog(__FILE__,__LINE__,"error",nError,data1,data2,data3)
	s2:="Your browser sent a request that this server could not understand. See RFC2616."+crlf
	do case
		case nError == HTTP_ERR_FILENOTFOUND
			ret_code := "Not found"
			s1:='The requested URL'
			if !empty(data1)
				s1+=' '+data1
			endif
			if !empty(data2)
				s1+=' '+data2
			endif
			if !empty(data3)
				s1+=' '+data3
			endif
			s1+=' was not found on this server.'
		case nError== HTTP_ERR_REQUESTTIMEOUT
			ret_code := "Request Timeout"
			s1+="Request Timeout."+crlf+s2
		case nError== HTTP_ERR_NOTIMPLEEMENTED
			ret_code := "Not Implemented"
			s1:="Method"
			if !empty(data1)
				s1+=" "+data1
			endif
			s1+=" not implemented."+crlf+s2
		case nError== HTTP_ERR_BADVERSION
			ret_code := "Bad Version"
			s1:="HTTP version"
			if !empty(data1)
				s1+=" "+toString(data1)
			endif
			s1+=" not supported."+crlf+s2
		case nError== HTTP_ERR_BADREQUEST
			ret_code := " Bad Request"
			s1:="Bad Request."
			if !empty(data1)
				s1+=", "+data1
			endif
			s1+=crlf+s2
		case nError== HTTP_ERR_INTERNALSERVERERROR
			ret_code:=' Internal Server Error'
			s1:='The server encountered an internal error or misconfiguration and was unable to complete your request.<P>'
			s1+= 'Please contact the server administrator,'
			s1+= self:admin
			s1+= ' and inform them of the time the error occurred,'
			s1+= ' and anything you might have done that may have'
			s1+= ' caused the error.<P>'
			if !empty(data1)
				s1+=data1
			endif
		otherwise
	endcase
	ret_code := alltrim(str(nError))+" "+ret_code
	data+='<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">'+crlf
	data+='<HTML><HEAD>'+crlf
	data+='<TITLE>'+ret_code+'</TITLE>'+crlf
	data+='</HEAD><BODY>'+crlf
	data+='<H1>'+ret_code+'</H1>'+crlf
	data+=s1+'<P> <HR>'+crlf
	data+='<ADDRESS> '+self:serverSign+'</ADDRESS>'+crlf
	data+='</BODY></HTML>'+crlf
	data+=crlf
	self:sendAnswer(ret_code,"text/html",@data,,"iso-8859-1",,.f.)
	//self:sendAnswer(ret_code)
return .t.
