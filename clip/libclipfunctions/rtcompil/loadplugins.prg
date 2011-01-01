function loadPlugIns(sFile)
	local compile := .f., loading := .t.
	local cmd,out:=space(0), err:=space(0)
	local ret,bTmp,mkey,eBlock
	local f1,f2
	if right(sFile,4) == ".prg"
		sFile := substr(sFile,1,len(sFile)-4)
	endif
	if !file(sFile+".prg")
		ret:=[Plugin not found:]+sFile
		return ret
	endif

	if !file(sFile+".po")
		compile := .t.
	else
		// mini make
		f1:=directory(sFile+".prg")
		f2:=directory(sFile+".po")
		if len(f1) > 0 .and. len(f2) >0
			if f1[1][3] > f2[1][3] .or. (f1[1][3] == f2[1][3] .and. f1[1][4] > f2[1][4] )
				compile:=.t.
			endif
		endif
	endif

	/* compile PRG-file */
	if compile
		ret := NIL
		cmd:=cliproot()+PATH_DELIM+"bin"+PATH_DELIM+"clip -v0 -p "+sFile+".prg"
		if syscmd(cmd,"",@out,@err)!=0
			ret:=out+err
		endif
		if !empty(out)
			memowrit(sFile+".out",out)
			chmod(sFile+".out","666")
		endif
		if !empty(err)
			ret:=err
		endif
		if ret != NIL
			ret:= [Error compiling plugin file:]+ret
			return ret
		endif
		chmod(sFile+".po","666")
	endif

	eBlock := errorBlock({ |err| break(err) })
	begin sequence
			ret := loadBlock(sFile+".po")
	recover
			ret := [Error loading plugin block from file:]+sFile+".po"
	end sequence
	errorBlock(eBlock)
return ret
