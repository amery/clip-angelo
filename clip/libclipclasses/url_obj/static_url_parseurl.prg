static function url_parseUrl(sUrl)
	local i,j,k,s
	sUrl:=alltrim(sUrl)
	sUrl:=strtran(sUrl,"//","/")
	i:=at("@",sUrl)
	i:=at("#",sUrl)
	if i>0   // URL anchor
		::anchor:=substr(sUrl,i+1)
		sUrl:=substr(sUrl,1,i-1)
	else
		::anchor:=NIL
	endif

	i:=at("?",sUrl)
	if i>0   // URL parameters
		::params:=substr(sUrl,i+1)
		sUrl:=substr(sUrl,1,i-1)
	else
		::params:=NIL
	endif
	if i>0			 //  user and password
	   j:=at("/",sUrl)
	   s=substr(sUrl,1,j-1)
	   if i>j .and. !empty(s)          // protocol and port
		k=at(":",s)
		k=iif(k>0,k,len(s)+1)
		::protocol:=substr(s,1,k-1)
		if len(s)-k>1	// port
			::port:=val(substr(s,k+1))
		endif
	   endif
	   sUrl:=substr(sUrl,j+1)
	   i:=at("@",sUrl)
	   s=substr(sUrl,1,i-1)
	   if !empty(s)          // user and password
		k=at(":",s)
		k=iif(k>0,k,len(s)+1)
		::user:=substr(s,1,k-1)
		::password:=substr(s,k+1)
	   endif
	   sUrl:=substr(sUrl,i+1)
	endif
	i:=at(":",sUrl)
	if i>0
		::protocol:=substr(sUrl,1,i-1)
		sUrl:=substr(sUrl,i+1)
	endif
	if !(lower(::protocol)=="file")
		i:=at("/",sUrl)
		if i==1
			sUrl:=substr(sUrl,2)
		endif
		i:=at("/",sUrl)
		if i>0
			::host:=substr(sUrl,1,i-1)
			sUrl:=substr(sUrl,i)
		else
			if !empty(sUrl) .and. sUrl!=::file
				::host:= sUrl
			endif
			return .t.
		endif

	endif
	i=rat("/",sUrl)
	if i>2
		::path:=substr(sUrl,1,i-1)
		if i!=len(sUrl)
			if !empty(sUrl)
				::file:=substr(sUrl,i+1)
			endif
		endif
	else
		if lower(::protocol)=="file"
			::path:=currdrive()+"/"+curdir()
		endif
		if !empty(sUrl)
			::file:=substr(sUrl,i+1)
		endif
	endif
return .t.
