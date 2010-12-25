function compileFile(Fname,flags,error)
	local cmd,out:=space(0), err:=space(0)
	flags:=iif(flags==NIL,"-p",flags)
	cmd:=cliproot()+PATH_DELIM+"bin"+PATH_DELIM+"clip "+flags+" "+fname
    //? cmd, syscmd(cmd,"",@out,@err), ferror(), ferrorstr()
	 syscmd(cmd,"",@out,@err)
	if .f. //syscmd(cmd,"",@out,@err)!=0
    	//? "A",out,err
		error:=out+err
		return .f.
	endif
	if !empty(err)
    	//? "B",out,err
		error:=err
		return .f.
	endif
	error:=""
return .t.
