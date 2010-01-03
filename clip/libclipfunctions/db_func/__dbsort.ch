function __dbSort(file,afields, ufor, uwhile, unext, rec, rest,rdd)
	local rsel:=select(), count:=0, rstruct,i,x1,x2
	local rfields:={},cset:=set(_SET_CANCEL,.f.)
	local err, flag_err:=.f.,  err_block:=errorblock({|x|break(x)})
	local allFlag:=.f., a,d,c,tmpfile,desc:=.f.
	local indString

	outlog(3,"sort to",file,"fields",afields,"for",ufor,"while",uwhile,"next",unext,"record",rec,"rest",rest)
	if empty(file)
		return 0
	endif
	begin sequence
		rstruct=dbstruct()
		if empty(afields)
			allFlag:=.t.
			for i=1 to len(rstruct)
				aadd(rfields, {rstruct[i][1], .f., .f.})
			next
		else
			for i=1 to len(afields)
				afields[i]:=upper(alltrim(afields[i]))
				a:=""; d:=.f.; c:=.f.
				if "/D" $ afields[i]
					d:=.t.
				endif
				if "/C" $ afields[i]
					c:=.t.
				endif
				x1:=at("/",afields[i])
				if x1>0
					a:=alltrim(substr(afields[i],1,x1-1))
				else
					a:=alltrim(afields[i])
				endif
				aadd(rfields,{a,d,c})
			next
		endif
		if len(rfields)==0
			set(_SET_CANCEL,cset)
			err=errorNew()
			err:description:="invalid argument"
			err:genCode:=EG_ARG
			//err:operation:="__DBCOPY"
			eval(errorblock(),err)
			//break(err)
		endif

		tmpfile="clip_sort_tmp_file"+alltrim(str(random(10000),4,0))
		count:=__dbcopy(tmpfile,{}, ufor, uwhile, unext, rec, rest,rdd)
		dbusearea(.t.,rdd,tmpfile,,.f.,,.t.)
		//use (tmpfile) exclusive via (rdd)
		indString:=""
		for i=1 to len(rfields)
			x1:=valType(&(rfields[i][1]))
			x2:=""
			do case
				case x1=="C"
					if rfields[i][3]
						x2:="LOWER("+rfields[i][1]+")"
					else
						x2:=rfields[i][1]
					endif
				case x1=="D"
					x2:="DTOS("+rfields[i][1]+")"
				case x1=="N"
					x2:="STR("+rfields[i][1]+")"
				case x1=="L"
					x2:="iif("+rfields[i][1]+',"1","0")'
			endcase
			if rfields[i][2]
				x2:="DESCEND("+x2+")"
			endif
			indString+=x2+"+"
		next
		indString:=substr(indString,1,len(indString)-1)
		if rdd==NIL
			rdd:=upper(rddSetDefault())
		endif
		if "C" $ substr(rdd,4) /* compaund index */
			//index on &indString tag a to tmpfile
			//ORDCONDSET(  ,  ,  ,  ,  ,  , RECNO() ,  ,  ,  , desc  )
			ORDCONDSET(  ,  ,  ,  ,  ,  , RECNO() ,  ,  ,  , )
			ORDCREATE( "tmpfile" , "a" , indString , {||&indString} ,  )
		else
			DBCREATEINDEX( "tmpfile" , indString , {||&indString} , IIF( .F. , .T. , NIL ) )
			//index on &indString to tmpfile
		endif
		set order to 1
		copy to (file)
		use
		ferase(tmpfile+".dbf")
		ferase(tmpfile+memoext())
		ferase(tmpfile+indexext())
	recover using err
		flag_err:=.t.
	end sequence

	select(rsel)
	set(_SET_CANCEL,cset)
	errorblock(err_block)
	if flag_err
	   err:operation+="/__DBSORT"
	   eval(errorblock(),err)
	   //break(err)
	endif
return count
