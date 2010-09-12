static function HTTP_defSets(self)
	self:hostname	:= getHostName()+"."+getDomainName()
	self:serverSoft	:= 'Kamache/'+alltrim(str(KAMACHE_VERSION))
	self:serverSign	:= self:serverSoft+' Server at '+self:hostname+' Port '+;
				alltrim(str(self:nPort))

	aadd(self:sets,{"SERVER_PROTOCOL","HTTP/1.1"})
	aadd(self:sets,{"HTTP_CONNECTION","Keep-Alive"})
	aadd(self:sets,{"SERVER_NAME",self:hostname})
	aadd(self:sets,{"SERVER_SOFTWARE",self:serverSoft})
	aadd(self:sets,{"SERVER_SIGNATURE",self:serverSign})
	aadd(self:sets,{"SERVER_PORT",alltrim(str(self:nPort))})
	aadd(self:sets,{"SERVER_ADDR",getHostByName(self:hostname)})
	//aadd(self:sets,{"HTTP_HOST",self:hostname})

return
