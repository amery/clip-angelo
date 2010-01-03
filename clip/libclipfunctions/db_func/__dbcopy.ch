function __dbCopy(file,rfields, ufor, uwhile, unext, rec, rest,rdd)
	local rsel:=select(),wsel, count:=0, wstruct:={},rstruct,i,x1,x2
	local wrec,rrec,fields:={},__alias:=tmpalias(),cset:=set(_SET_CANCEL,.f.)
	local err, flag_err:=.f.,  err_block:=errorblock({|x|break(x)})
	local dflag, allFlag:=.f., rawFlag
	local __bfor,__bwhile,__rFor,__rWhile

	outlog(3,"copy to",file,"fields",rfields,"for",ufor,"while",uwhile,"next",unext,"record",rec,"rest",rest)
	begin sequence
		rawFlag:=empty(rfields) .and. empty(rdd) .and. !clip_ismemo()
		rstruct=dbstruct()
		if empty(rfields)
			rfields := {}
			allFlag:=.t.
			for i=1 to len(rstruct)
				aadd(rfields, rstruct[i][1])
			next
		endif
		for i=1 to len(rfields)
			rfields[i]:=upper(alltrim(rfields[i]))
			x1:=ascan(rstruct,{|x|x[1]==rfields[i]})
			if x1!=0
				aadd(wstruct,rstruct[x1])
				aadd(fields,hashstr(rfields[i]))
			endif
		next
		if len(fields)==0
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="invalid argument"
			err:genCode:=EG_ARG
			//err:operation:="__DBCOPY"
			eval(errorblock(),err)
			//break(err)
		endif

		if valtype(rec)=="N"
			goto rec
			rest=.t.
			unext=1
		endif
		if uwhile!=NIL
			rest := .t.
		endif

		if empty(rest) .and. unext==NIL //rest==NIL .or. !rest
			goto top
		endif
		unext=iif(valtype(unext)!="N",lastrec(),unext)

		if valtype(ufor)=="C"
			__bfor:="{||"+ufor+"}"
			__bfor:=&__bfor
			__rfor:=__bfor
		else
			if valtype(ufor)!="B"
				__bfor:={||.t.}
			else
				__bfor:=ufor
				__rfor:=__bfor
			endif
		endif
		if valtype(uwhile)=="C"
			__bwhile:="{||"+uwhile+"}"
			__bwhile:=&__bwhile
			__rwhile:=__bwhile
		else
			if valtype(uwhile)!="B"
				__bwhile:={||.t.}
			else
				__bwhile:=uwhile
				__rwhile:=__bwhile
			endif
		endif

		select 0
		dbcreate(file,wstruct,rdd)
		use (file) NEW exclusive alias (__alias) via (rdd)
		wsel:=select()
		if rawFlag
			select(rsel)
			count:=__dbcopyRaw(wsel,__rfor,__rwhile,unext)
		else
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
				if allflag
					wrec:=rrec
				else
					for i=1 to len(fields)
						wrec[fields[i]]=rrec[fields[i]]
					next
				endif
				select(wsel)
				dbappend(,wrec)
				if dflag
					delete
				endif
				select(rsel)
				if count < unext
					skip
				endif
			enddo
		endif
		select(wsel)
		use
	recover using err
		flag_err:=.t.
	end sequence

	select(rsel)
	set(_SET_CANCEL,cset)
	errorblock(err_block)
	if flag_err
	   err:operation+="/__DBCOPY"
	   eval(errorblock(),err)
	   //break(err)
	endif
return count
