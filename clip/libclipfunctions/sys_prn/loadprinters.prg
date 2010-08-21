**********************************************
* load CLIPROOT/etc/printers.ini and return data
**********************************************
function loadPrinters()
	local oIni,ret:=map()
	local f,p,mIni,i
	f:=cliproot()+PATH_DELIM+"etc"+PATH_DELIM+"printers.ini"
	if !file(f)
		outlog(3,"file not found:",f)
		f:=PATH_DELIM+"home"+PATH_DELIM+getenv("USER")+PATH_DELIM+".clip"+PATH_DELIM+"printers.ini"
		if !file(f)
			outlog(3,"file not found:",f)
			ret:queue := printQueueNew("view",1,"A4")
			ret:printers := {}
			*
			p := printerNew("view",,[View printer output in text editor],;
				host_charset(),,"{|filename|medit(fileName)}" )
			aadd(ret:printers,p)
			*
			p := printerNew("lpr",,[Printing to main printer on your host],;
				host_charset(),"lpr" )
			aadd(ret:printers,p)
			return ret
		endif
	endif
	ret:printers := {}
	oIni := iniFileNew(f)
	oIni:load()
	mIni := oIni:sections()
	for i=1 to len(mIni)
		if mIni[i] == "DEFAULT"
			ret:queue := ;
			printQueueNew( 	oIni:getValue(mIni[i],"name"),;
					oIni:getValue(mIni[i],"copies"),;
					oIni:getValue(mIni[i],"psize"),;
					oIni:getValue(mIni[i],"orientation"), ;
					oIni:getValue(mIni[i],"begpage"), ;
					oIni:getValue(mIni[i],"endpage") ;
			)
			loop
		endif
		p := printerNew ( ;
		     oIni:getValue(mIni[i],"name"),;
		     oIni:getValue(mIni[i],"type"),;
		     oIni:getValue(mIni[i],"description"),;
		     oIni:getValue(mIni[i],"charset"),;
		     oIni:getValue(mIni[i],"script"),;
		     oIni:getValue(mIni[i],"block") ;
		)
		aadd(ret:printers, p)
	next
	for i=1 to len(ret:printers)
		if ret:printers[i]:name == ret:queue:name
			exit
		endif
	next
	/* move default printer to first position in list*/
	if i<=len(ret:printers)
		p := ret:printers[i]
		adel(ret:printers,i)
		ains(ret:printers,1)
		ret:printers[1] := p
	endif
return ret
