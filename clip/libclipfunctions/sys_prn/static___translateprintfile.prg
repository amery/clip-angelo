************************
static function __translatePrintFile(infile, outFile, oPrn, charset)
	local nFin, nFout,buf,i1,i2,s1,s2,vcode,rCode
	nFin := fopen(inFile,FO_READ)
	if nFin < 0
		outlog(3,"Error open source printing file:",ferror(),ferrorstr())
		return .f.
	endif
	nFout := fcreate(outFile)
	if nFout < 0
		outlog(3,"Error open target printing file:",ferror(),ferrorstr())
		fclose(nFin)
		return .f.
	endif
	while !fileeof(nFin)
	       buf:=filegetstr(nFin)
	       buf:=translate_charset(host_charset(),charset,buf)
	       while .t.
			i1 := at("\",buf)
			if i1 <=0
				exit
			endif
			s1 := substr(buf,1,i1-1)
			s2 := substr(buf,i1+1)
			i2 := at("\",s2)
			if i2 <=0
				exit
			endif
			vCode := substr(s2,1,i2-1)
			s2:= substr(s2,i2+1)
			rCode := oPrn:getValue("vcodes",vCode)
			buf:=s1
			for i1=1 to len(rCode)
				buf += chr(rCode[i1])
			next
			buf+=s2
	       end
	       fwrite(nFout,buf+CRLF)
	end
	fclose(nFin)
	fclose(nFout)
return
