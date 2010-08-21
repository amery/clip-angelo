static function smtp_send(sFrom,sTo,sData)
	local ret,i,buf := space(BUF_LEN)
	local fields := ""
	local content_type
	local boundary := "CLIP - Clipper compatible compiler"
	local fname,pos1,pos2,pos,ct
	local base64 := .t.

	::error:=""
	if ::handle==NIL
		::error:=[Connect not activated]
		return .f.
	endif
	fields += "From: "+sFrom+::LF
	fields += "To: "+sTo+::LF
	if empty(::attachments)
		content_type := "text/plain; charset="+host_charset()
	else
		fields += "MIME-Version: 1.0"+::LF
		content_type := 'multipart/mixed; boundary="'+boundary+'"'
		sData := "--"+boundary+::LF;
			+"Content-Type: text/plain; charset="+host_charset()+::LF;
			+"Content-Transfer-Encoding: 8bit"+::LF+::LF+sData+::LF
		for i=1 to len(::attachments)
			sData += "--"+boundary+::LF
			pos := rat(".",::attachments[i])
			if pos >0
				ct := mimeTypeGet(substr(::attachments[i],pos+1))
			endif
			if empty(ct)
				ct := "application/octet-stream"
			endif
			if upper(left(ct,5)) == "TEXT/"
				base64 := .f.
			endif
			sData += "Content-Type: "+ct+'; name="'
			pos1 := rat("/",::attachments[i])
			pos2 := rat("\",::attachments[i])
			pos := max(pos1,pos2)
			fname := substr(::attachments[i],pos+1)
			sData += fname+'"'+::LF
			sData += 'Content-Disposition: attachment; filename="'+fname+'"'+::LF
			if base64
				sData += "Content-Transfer-Encoding: base64"+::LF
			endif
			sData += ::LF
			if base64
				//sData += base64encode(filestr(::attachments[i]))+::LF
				sData += base64encode(memoread(::attachments[i]))+::LF
			else
				//sData += filestr(::attachments[i])+::LF
				sData += memoread(::attachments[i])+::LF
			endif
		next
		sData += "--"+boundary+"--"+::LF
	endif
	fields += "Content-Type: "+content_type+::LF
	for i=1 to len(::fields)
		fields += ::fields[i][1]+": "+::fields[i][2]+::LF
	next
	if !empty(fields)
		fields+=::LF
	endif
	/* check connect with server */
	tcpwrite(::handle,"NOOP"+::EOL,,::timeout)
	if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
		::error := ferrorstr()+" "+substr(buf,1,ret)
		tcpclose(::handle)
		return .f.
	endif
	if !empty(sFrom)
		ret:=tcpwrite(::handle,"MAIL FROM:"+sFrom+""+::EOL,,::timeout)
		if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
			::error:=substr(buf,1,ret)
			return .f.
		endif
	endif
	if !empty(sTo)
		ret:=tcpwrite(::handle,"RCPT TO:"+sTo+""+::EOL,,::timeout)
		if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
			::error:=substr(buf,1,ret)
			return .f.
		endif
	endif
	sData := fields+sData
	if !empty(sData)
		ret:=tcpwrite(::handle,"DATA"+::EOL,,::timeout)
		if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
			::error:=substr(buf,1,ret)
			return .f.
		endif
        	ret:=tcpwrite(::handle,sData,,::timeout)
	endif
	ret:=tcpwrite(::handle,::EOL+"."+::EOL,,::timeout)
	if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
		::error:=substr(buf,1,ret)
		return .f.
	endif
return .t.
