function __dbCopyDelim(file,fdelim,rfields, ufor, uwhile, unext, rec, rest)
	local rsel:=select(),hfile, count:=0, wstruct:={},rstruct,i,x1,x2
	local wrec,rrec,fields:={},__alias:=tmpalias(),cset:=set(_SET_CANCEL,.f.)
	local rdelim:=",", err, flag_err:=.f.,  err_block:=errorblock({|x|break(x)})
	local __bfor,__bwhile

	fdelim=iif(fdelim==NIL,'"',fdelim)
	if lower(fdelim)=="blank"
		fdelim=""
		rdelim=" "
	endif

	outlog(3,"copy to delimited with",fdelim,file,"fields",rfields,"for",ufor,"while",uwhile,"next",unext,"record",rec,"rest",rest)

	if !(".txt" $ file .or. "." $ file)
		file+=".txt"
	endif
	begin sequence
		hfile:=fcreate(file)
		if hfile<0
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="cannot open file:"+file
			err:genCode:=EG_OPEN
			//err:operation:="__DBCOPYDELIM"
			eval(errorblock(),err)
			//break(err)
		endif

		rstruct=dbstruct()
		if empty(rfields)
			rfields := {}
			for i=1 to len(rstruct)
				aadd(rfields, rstruct[i][1])
			next
		endif
		for i=1 to len(rfields)
			rfields[i]:=upper(alltrim(rfields[i]))
			x1:=ascan(rstruct,{|x|x[1]==rfields[i]})
			if x1!=0
				aadd(wstruct,rstruct[x1])
				aadd(fields,hashstr(upper(rfields[i])))
			endif
		next
		if len(fields)==0
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="invalid argument"
			err:genCode:=EG_ARG
			//err:operation:="__DBCOPYDELIM"
			eval(errorblock(),err)
			//break(err)
		endif

		if valtype(rec)=="N"
			goto rec
			rest=.t.
			unext=1
		endif
		if uwhile != NIL
			rest := .t.
		endif
		if empty(rest) .and. unext==NIL //rest==NIL .or. !rest
			goto top
		endif
		unext=iif(valtype(unext)!="N",lastrec(),unext)

		if valtype(ufor)=="C"
			__bfor:="{||"+ufor+"}"
			__bfor:=&__bfor
		else
			if valtype(ufor)!="B"
				__bfor:={||.t.}
			else
				__bfor:=ufor
			endif
		endif

		if valtype(uwhile)=="C"
			__bwhile:="{||"+uwhile+"}"
			__bwhile:=&__bwhile
		else
			if valtype(uwhile)!="B"
				__bwhile:={||.t.}
			else
				__bwhile:=uwhile
			endif
		endif

		select(rsel)
		wrec:=map()


		do while !eof() .and. count<unext .and. eval(__bwhile)
			count++
			if !eval(__bfor)
				skip
				loop
			endif
			rrec:=dbread()
			wrec:=""
			for i=1 to len(fields)
				x1=rrec[fields[i]]
				do case
					case valtype(x1)=="C"
						if !empty(set("DBF_CHARSET"))
							x1=translate_charset(host_charset(),set("DBF_CHARSET"),x1)
						endif
						wrec+=fdelim+alltrim(x1)+fdelim+rdelim
					case valtype(x1)=="N"
						wrec+=alltrim(str(x1))+rdelim
					case valtype(x1)=="D"
						wrec+=dtos(x1)+rdelim
					case valtype(x1)=="L"
						wrec+=iif(x1,"T","F")+rdelim
				endcase
			next
			wrec:=left(wrec,len(wrec)-len(rdelim))
			fwrite(hfile,wrec+CRLF)
			if count<unext
				skip
			endif
		enddo

	recover using err
		flag_err:=.t.
	end sequence

	fclose(hfile)
	select(rsel)
	set(_SET_CANCEL,cset)
	errorblock(err_block)
	if flag_err
		err:operation+="/__DBCOPYDELIM"
		eval(errorblock(),err)
		//break(err)
	endif
return count
