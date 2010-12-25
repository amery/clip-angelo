function httpServiceNew(oIni)
	local obj:=map()
	obj:classname	:= "HTTPSERVICE"
	obj:cHost	:= HTTP_DEFHOST
	obj:nPort	:= KAMACHE_DEFPORT
	obj:nTimeOut	:= TCP_CONNECT_TIMEOUT
	obj:nIOtimeOut	:= TCP_IO_TIMEOUT
	obj:nVersion	:= KAMACHE_VERSION
	obj:DocRoot	:= KAMACHE_DOCROOT
	obj:cgiBin	:= KAMACHE_CGIBIN
	obj:modClip	:= KAMACHE_MODCLIP
	obj:modCache	:= ""
	obj:aliases	:= {}
	obj:ssi		:= {}
	obj:fileicons	:= map()
	obj:mimeicons	:= map()
	obj:hostname	:= getHostName()+"."+getDomainName()
	obj:admin	:= "webmaster@"+obj:hostname
	obj:serverSoft	:= ""
	obj:serverSign	:= ""
	obj:dateFormat	:= "dd.mm.yyyy"

	obj:nConnect	:= -1
	obj:cLine	:= ""
	obj:buffer	:= ""
	obj:command	:= ""
	obj:error	:= ""
	obj:errno	:= 0
	obj:nSended	:= 0
	obj:nReceived	:= 0
	obj:oIni	:= oIni
	obj:sets	:= {}

	_recover_httpservice(obj)

	obj:fromIni(oIni)
	obj:defSetting()
return obj
