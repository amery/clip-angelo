function compileString(str,error)
	local block:={|| NIL }, file:=tmpfile(),err
	err:=atr("/",file)
	err:=iif(err<=0,0,err)
	file:=substr(file,err+1)
	memowrit(file+".prg",str)
	compileFile(file+".prg","-p",@err)
	ferase(file+".prg")
	if !empty(err)
		error:=err
	else
		block:=loadBlock(file+".po")
	endif
	ferase(file+".po")
return block
