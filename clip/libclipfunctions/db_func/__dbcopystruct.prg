function __dbCopyStruct(fname,rfields)
	local i,ret,fields:={},stru,err
	if empty(rfields)
		rfields := {}
		outlog(3,"copystruct",fname,dbstruct())
		dbcreate(fname,dbstruct())
		return
	endif
	outlog(3,"copystruct",fname,rfields)
	stru:=dbstruct()
	for i=1 to len(rfields)
		rfields[i]:=upper(alltrim(rfields[i]))
		ret:=ascan(stru,{|x|x[1]==rfields[i]})
		if ret!=0
			aadd(fields,stru[ret])
		endif
	next
	if len(fields)==0
		err=errorNew()
		err:description:="invalid argument"
		err:genCode:=EG_ARG
		eval(errorblock(),err)
	else
		dbcreate(fname,fields)
	endif
return
