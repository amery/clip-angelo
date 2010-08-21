**********************************************************
* check structure of DBF file for "struct" and remake DBF for new structure.
**********************************************************
function dbCheckStructure(file,struct,rdd)
	local err,eblock:=errorblock({|e| break(e) } )
	local ret:=0, sel:=select()
	local app_flag:=.f. , dstru, i, j, sret
	local file1, file2, path, tmpname

	select 0

	if !file(file)
		begin sequence
			dbcreate(file,struct,rdd)
		recover using err
			ret := EG_CREATE
		end sequence
		errorBlock(eblock)
		select(sel)
		return ret
	endif

	begin sequence
		use (file) shared via (rdd)
	recover using err
		ret := EG_OPEN
	end sequence
	if neterr()
		ret:= EG_OPEN
	endif
	if ret != 0
		errorBlock(eblock)
		select(sel)
		return ret
	endif
	/* header correct */
	append blank
	rlock()
	delete
	use

	begin sequence
		use (file) exclusive via (rdd)
	recover using err
		ret := EG_OPEN
	end sequence
	if neterr()
		ret:= EG_OPEN
	endif
	if ret != 0
		use
		errorBlock(eblock)
		select(sel)
		return ret
	endif

	dstru:=dbstruct()
	for j=1 to len(struct)
		if j>len(dstru)
			app_flag := .t.
			exit
		endif
		struct[j][1] := alltrim(upper(struct[j][1]))
		struct[j][2] := alltrim(upper(struct[j][2]))
		sret=ascan(dstru,{|x|x[1]==struct[j][1]})
		if sret==0
			app_flag := .t.
			exit
		endif
		if dstru[sret][2]==struct[j][2]
		else
			ret:=EG_DATATYPE
			exit
		endif
	next
	if ret != 0
		use
		errorBlock(eblock)
		select(sel)
		return ret
	endif
	for j=1 to len(dstru)
		sret=ascan(struct,{|x|x[1]==dstru[j][1]})
		if sret==0
			aadd(struct, dstru[j])
			app_flag := .t.
			loop
		endif
		if struct[sret][3]<=dstru[j][3]
			struct[sret][3]=dstru[j][3]
			struct[sret][4]=dstru[j][4]
			app_flag := .t.
		endif
		if struct[sret][3]>dstru[j][3]
			app_flag := .t.
		endif
		if struct[sret][4]<=dstru[j][4]
			struct[sret][4]=dstru[j][4]
			app_flag := .t.
		endif
		if struct[sret][4]>dstru[j][4]
			app_flag := .t.
		endif
	next

	use

	path := makepath(file)
	j := atr(PATH_DELIM,path)
	if j ==0
		ret := EG_CREATE
	else
		path := substr( path, 1, j)
	endif
	tmpname=path+"clip"+alltrim(str(random(10000),4,0))
	if app_flag .and. ret == 0
		file1=file
		file2=tmpname+".dbf"
		delete file (file2)
		rename (file1) to (file2)
		if (file(file1))
			ret := EG_WRITE
		endif
	endif
	if app_flag .and. ret == 0
		file1=substr(file,1,atr(".",file))+"dbt"
		if file(file1)
			file2=tmpname+".dbt"
			delete file (file2)
			if file(file1)
				rename (file1) to (file2)
			endif
			if (file(file1))
				ret := EG_WRITE
			endif
		endif
		file1=substr(file,1,atr(".",file))+"fpt"
		if file(file1)
			file2=tmpname+".fpt"
			delete file (file2)
			if file(file1)
				rename (file1) to (file2)
			endif
			if (file(file1))
				ret := EG_WRITE
			endif
		 endif
	endif
	if ret != 0
		errorBlock(eblock)
		select(sel)
		return ret
	endif
	if app_flag
		begin sequence
			dbcreate(file,struct,rdd)
			use (file) exclusive via rdd
		recover using err
			ret := EG_CREATE
		end sequence
		if neterr()
			ret:= EG_CREATE
		endif
		if ret != 0
			use
			errorBlock(eblock)
			select(sel)
			return ret
		endif
		begin sequence
#ifdef CLIPPER50_COMPATIBLE
			append from (tmpname)
#else
			append from (tmpname) via (rdd)
#endif
		recover using err
			ret := EG_WRITE
		end sequence
		use
	endif
	errorBlock(eblock)
	select(sel)
	delete file (tmpname+".dbf")
	delete file (tmpname+".dbt")
	delete file (tmpname+".fpt")
return  ret
