static function HTTP_close(self)
	//local cBuf := space(10)
	if self:nConnect > 0
		/*
		outlog(__FILE__,__LINE__,self:nConnect,seconds())
		tcpRead( self:nConnect, @cbuf, 10, 100 )
		*/
	    //	outlog(__FILE__,__LINE__,self:nConnect,seconds())
		tcpClose(self:nConnect)
	    //	outlog(__FILE__,__LINE__,self:nConnect,seconds())
		self:nConnect := -1
	endif
return .t.
