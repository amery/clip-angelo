static function HTTP_sendDir(self,url,cmd,m)
	local i,x,y,ext,mime,icon
	local maxlen:=17,prevDir
	local crlf := chr(13)+chr(10)
	local data := crlf

	data += '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN"'+crlf
	data += '"http://www.w3.org/TR/REC-html40/loose.dtd">'+crlf
	data += '<html><head><title>Index of '+url+'</title></head>'+crlf
	data += '<body bgcolor="#ffffff" text="#000000">'+crlf
	data += '<table><tr><td bgcolor="#ffffff" class="title">'+crlf
	data += '<font size="+3" face="Helvetica,Arial,sans-serif">'+crlf
	data += '<b>Index of '+url+'</b></font>'+crlf

	data += '</td></tr></table>'+crlf

	for i=1 to len(m)
		maxlen := max(maxlen,len(m[i][1]))
	next
	if cmd == "N=D"
		asort(m,,,{|x,y|x[1]<=y[1]})
	elseif cmd == "M=A"
		asort(m,,,{|x,y|dtos(x[3])+x[4]<=dtos(y[3])+y[4]})
	elseif cmd == "S=A"
		asort(m,,,{|x,y|x[5]=="D" .or. x[2]<=y[2]})
	endif

	data += '<pre><img border="0" src="/icons/blank.gif" ALT="[INFO]">'
	data += '<a href="?N=D">Name</a>'+space(maxlen-3)
	data += '<a href="?M=A">Last modified</a>            '
	data += '<a href="?S=A">Size</a>  <a href="?D=A">Description</a>'+crlf
	data += '<hr noshade align="left" width="80%">'+crlf
	*
	url := strtran(url,"//","/")
	if len(url)>1 .and. right(url,1) == "/"
		url := left(url,len(url)-1)
	endif
	i := atr("/",url)
	if i>0
		prevdir := left(url,i)
	else
		prevDir := "/"
	endif
	data += '<img border="0" src="/icons/back.gif" alt="[DIR]"> '
	data += '<a href="'+prevdir+'">Parent Directory</a>'+space(maxlen-15)
	i := ascan(m,{|x|x[1]==".."})
	if i>0
		data += ascDateTime(m[i][3],m[i][4],2)+'     -'+crlf
	else
		data += ascDateTime(,,,2)+'     -'+crlf
	endif
	for i=1 to len(m)
		if m[i][1]=="." .or. m[i][1]==".."
			loop
		endif
		x := atr(".",m[i][1])
		ext := upper(iif(x>0, substr(m[i][1],x+1),""))
		if ext $ self:fileicons
			icon := self:fileicons[ext]
		else
			mime = mimeTypeGet(ext)
			x := at("/",mime)
			ext := upper(iif(x>0, left(mime,x-1),""))
			if ext $ self:mimeicons
				icon := self:mimeicons[ext]
			else
				icon := "unknown.gif"
			endif
		endif
		data += '<img border="0" src="/icons/'
		if m[i][5] == "D"
			data+='folder.gif" alt="[DIR ]"> '
		else
			data+=icon+'" alt="['+padr(ext,4)+']"> '
		endif
		data += '<a href="'+url+'/'+m[i][1]+'">'+m[i][1]+'</a>'+space(maxlen-len(m[i][1]))
		data += ascDateTime(m[i][3],m[i][4],2)
		data += str(m[i][2]/1000,6,0)+"K"
		data += crlf
	next
	data += '</pre><hr noshade align="left" width="80%">'+crlf
	data +='<ADDRESS> '+self:serverSign+'</ADDRESS>'+crlf
	data += '</body></html>'+crlf
	//self:sendAnswer("200 OK","text/html",@data,ascDateTime())
	self:sendAnswer("200 OK","text/html",@data,,,-1)

