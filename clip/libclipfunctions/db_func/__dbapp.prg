function __dbApp(file,rfields, ufor, uwhile, unext, rec, rest, rdd)
	local wsel:=select(),rsel, count:=0, rstruct,wstruct:=dbstruct(),i,x1,x2
	local wrec,rrec,fields:={},__alias:=tmpalias(),cset:=set(_SET_CANCEL,.f.)
		local err, flag_err:=.f., err_block:=errorblock({|x|break(x)})
		local dflag
	local __bfor,__bwhile

	outlog(3,"append",alias(),"from",file,"fields",rfields,"for",ufor,"while",uwhile,"next",unext,"record",rec,"rest",rest)


	begin sequence
		use (file) NEW shared alias (__alias) via (rdd)
		rsel:=select()

		if valtype(rec)=="N"
			goto rec
			rest=.t.
			unext=1
		endif
		if uwhile != NIL
			rest := .t.
		endif
		if empty(rest) .and. unext == NIL //rest==NIL .or. !rest
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
			x2:=ascan(wstruct,{|x|x[1]==rfields[i]})
			if x1!=0 .and. x2!=0
				aadd(fields,hashstr(rfields[i]))
			endif
		next

		if len(fields)==0
			select(rsel)
			use
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="invalid argument"
			err:genCode:=EG_ARG
			//err:operation:="__DBAPP"
			eval(errorblock(),err)
			//break(err)
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
			dflag:=deleted()
			for i=1 to len(fields)
				wrec[fields[i]]=rrec[fields[i]]
			next
			select(wsel)
			dbappend(,wrec)
			if dflag
				delete
			endif
			select(rsel)
			if count<unext
				skip
			endif
		enddo

		select(rsel)
		use
	recover using err
		flag_err:=.t.
	end sequence
	select(wsel)
	set(_SET_CANCEL,cset)
	errorblock(err_block)
	if flag_err
		   err:operation+="/__DBAPP"
		   eval(errorblock(),err)
		   //break(err)
	endif
return count
