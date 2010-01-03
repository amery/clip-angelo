#ifdef 0
**********************************************************
function __dbTotal(file,ukey,rfields, ufor, uwhile, unext, rec, rest, rdd)
	local rsel:=select(),wsel, count:=0, wstruct:={},rstruct,i,x1,x2
	local wrec,rrec,fields:={},__alias:=tmpalias(),cset:=set(_SET_CANCEL,.f.)
		local cur_val, err, flag_err:=.f.,  err_block:=errorblock({|x|break(x)})
	local __bkey,__bfor,__bwhile

	outlog(3,"__dbTotal",file,alias(),ukey,"fields",rfields,"for",ufor,"while",uwhile,"next",unext,"record",rec,"rest",rest)
	begin sequence
		rstruct=dbstruct()
		for i=1 to len(rfields)
			rfields[i]:=upper(alltrim(rfields[i]))
			x1:=ascan(rstruct,{|x|x[1]==rfields[i]})
			if x1!=0 .and. rstruct[x1][2] != "M"
				aadd(wstruct,rstruct[x1])
				aadd(fields,hashstr(rfields[i]))
			endif
		next
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

		if empty(ukey)
			   ukey=indexkey()
		endif
		if empty(ukey)
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="invalid argument"
			err:genCode:=EG_ARG
			//err:operation:="__DBTOTAL"
			eval(errorblock(),err)
			//break(err)
		endif

		if valtype(ukey)=="C"
			__bkey:="{||"+ukey+"}"
			__bkey:=&__bkey
		else
			if valtype(ukey)!="B"
				__bkey:={||.t.}
			else
				__bkey:=ukey
			endif
		endif

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

		select 0
		dbcreate(file,rstruct,rdd)
		use (file) NEW exclusive alias (__alias) via (rdd)
		wsel:=select()

		select(rsel)
		wrec:=dbread()
		cur_val:=eval(__bkey)
		do while !eof() .and. count<unext .and. eval(__bwhile)
			count++
			if !eval(__bfor)
				skip
				loop
			endif
			rrec:=dbread()
			if cur_val==eval(__bkey)
			   for i=1 to len(fields)
				wrec[fields[i]]+=rrec[fields[i]]
			   next
			else
			   select(wsel)
			   dbappend(,wrec)
			   select(rsel)
			   wrec=dbread()
			   cur_val:=eval(__bkey)
			endif
			if count<unext
				skip
			endif
		enddo
		select(wsel)
		dbappend(,wrec)
		use
	recover using err
		flag_err:=.t.
	end sequence

	select(rsel)
	set(_SET_CANCEL,cset)
	errorblock(err_block)
	if flag_err
		   err:operation+="/__DBTOTAL"
		   eval(errorblock(),err)
		   //break(err)
	endif
return count
#endif
