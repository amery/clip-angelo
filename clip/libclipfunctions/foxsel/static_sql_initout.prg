static function sql_initOut()
	local i,ii,iii,aname,data,fname,mstru:={}
        local s
        i:=::to
        if i==SQL_TO_CURSOR .or. i==SQL_TO_DBF
        	for ii=1 to len(::from)
        		aname:=::from[ii]
        		rddGoTop(aname:hDb)
                        data:=rddRead(aname:hDB)
                	private &(aname:file):=data
                	s:=aname:dbStruct
                        for iii in s
                        	private &(iii[1]):=data[ iii[1] ]
                        next
        	next
        	for i in ::fields
			data:=eval(i:block)
                	//aadd(mstru,{i:xname,valtype(data),len(data),dec(data)})
                        i:len := max(i:len,len(data))
                        i:dec := max(i:dec,dec(data))
                	aadd(mstru,{i:xname,valtype(data),i:len,i:dec})
        	next
        endif
        set console (::console)
	do case
                case ::to==SQL_TO_NONE
                        ::outBuffer:=""
        	case ::to==SQL_TO_PRINTER
                	set device to printer
                        set printer on
                        ::outBuffer:=""
        	case ::to==SQL_TO_FILE
                	if ::additive .and. file(::filename)
                		::hfile:=fopen(::filename,FO_WRITE)
                        else
                        	::hfile:=fcreate(::filename,FC_NORMAL)
                        endif
                        if ::hfile<0
                        	fx_sql_error("Cannot open output file:"+::filename,__LINE__)
                        endif
                        if ::additive
                        	fseek(::hFile,0,FS_END)
                        endif
                        ::newLine :=@file_newLine()
                        ::out    :=@file_out()
                        ::heading:=.f.
                        ::outBuffer:=""
        	case ::to==SQL_TO_CURSOR
                	fname:=tmpfile()
                        dbcreate(fname,mstru)
                        use (fname) alias (::fileName) exclusive temporary new
                        ::newLine :=@tbl_newLine()
                        ::out    :=@tbl_out()
                        ::heading:=.f.
                        ::outBuffer:=map()
        	case ::to==SQL_TO_DBF
                        dbcreate(::filename,mstru)
                        use (::filename) new
                        ::newLine :=@tbl_newLine()
                        ::out    :=@tbl_out()
                        ::heading:=.f.
                        ::outBuffer:=map()
        	case ::to==SQL_TO_TABLE
                        dbcreate(::filename,mstru)
                        use (::filename) new
                        ::newLine :=@tbl_newLine()
                        ::out    :=@tbl_out()
                        ::heading:=.f.
                        ::outBuffer:=map()
        	case ::to==SQL_TO_ARRAY
                	&(::filename):={}
                        ::newLine  :=@arr_newLine()
                        ::out     :=@arr_out()
                        ::heading :=.f.
                        ::outBuffer  :={}
        	case ::to==SQL_TO_MAP
                	&(::filename):=map()
                        data:=&(::filename)
        		for i in ::fields
				data[i:xname]:={}
                        next
                        ::newLine  :=@map_newLine()
                        ::out     :=@map_out()
                        ::heading :=.f.
                        ::outBuffer  :=map()
        	case ::to==SQL_TO_ANYMAP
                        ::newLine  :=@anymap_newLine()
                        ::out     :=@anymap_out()
                        ::heading :=.f.
                	::outBuffer  :=map()
        endcase
return
