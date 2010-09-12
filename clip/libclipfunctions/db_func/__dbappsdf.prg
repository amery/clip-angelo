function __dbAppSDF(file,rfields, ufor, uwhile, unext, rec, rest, rdd)
	local wsel:=select(),rsel, count:=0, rstruct,wstruct:=dbstruct(),i,x1,x2
	local wrec,rrec,fields:={},__alias:=tmpalias(),cset:=set(_SET_CANCEL,.f.), hfile
	local rstr,types:={},lens:={}, err, flag_err:=.f., err_block:=errorblock({|x|break(x)})
	local __bfor,__bwhile
	local __pvar,__pname

	outlog(3,"append sdf",alias(),"from",file,"fields",rfields,"for",ufor,"while",uwhile,"next",unext,"record",rec,"rest",rest)

	if !(".txt" $ file .or. "." $ file)
		file+=".txt"
	endif

	begin sequence
		hfile:=fopen(file,FO_READ)
		if hfile<0
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="cannot open file:"+file
			err:genCode:=EG_OPEN
			//err:operation:="__DBAPPSDF"
			eval(errorblock(),err)
			//break(err)
		endif


		if valtype(rec)=="N"
			//goto rec
			for i=1 to rec
				filegetstr(hfile,MAX_STR)
			next
			rest=.t.
			unext=1
		endif
		unext=iif(valtype(unext)!="N",MAX_RECORDS,unext)

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

		if empty(rfields)
			rfields := {}
			for i=1 to len(wstruct)
				aadd(rfields,wstruct[i][1])
			next
		endif

		for i=1 to len(rfields)
			rfields[i]:=upper(alltrim(rfields[i]))
			x1:=ascan(wstruct,{|x|x[1]==rfields[i]})
			if x1!=0
				aadd(fields,hashstr(rfields[i]))
				aadd(types,wstruct[x1][2])
				aadd(lens,wstruct[x1][3])
				__pname=rfields[i]
				private &__pname
			endif
		next

		if len(fields)==0
			fclose(hfile)
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="invalid argument"
			err:genCode:=EG_ARG
			//err:operation:="__DBAPPSDF"
			eval(errorblock(),err)
			//break(err)
		endif

		wrec:=map()
		do while !fileeof(hfile) .and. count<unext
			count++
			select 0
			rstr:=filegetstr(hfile,MAX_STR)
			if empty(rstr)
				loop
			endif
			rrec:=1
			for i=1 to len(fields)
				x1=substr(rstr,rrec,lens[i])
				rrec+=lens[i]
				do case
					case types[i]=="C"
						if !empty(set("DBF_CHARSET"))
							x1=translate_charset(set("DBF_CHARSET"),host_charset(),x1)
						endif
					case types[i]=="N"
						x1=val(x1)
					case types[i]=="D"
						x1=stod(x1)
					case types[i]=="L"
						x1=(x1=="T")
				endcase
				__pname=rfields[i]
				&__pname=x1
			next
			if !eval(__bfor)
				loop
			endif
			if !eval(__bwhile)
				exit
			endif
			for i=1 to len(fields)
				__pname=rfields[i]
				wrec[fields[i]]=&__pname
			next
			select(wsel)
			dbappend(,wrec)
		enddo

	fclose(hfile)
	recover using err
		flag_err:=.t.
	end sequence
	commit
	select(wsel)
	set(_SET_CANCEL,cset)
	errorblock(err_block)
	if flag_err
		err:operation+="/__DBAPPSDF"
		eval(errorblock(),err)
		//break(err)
	endif
return count
