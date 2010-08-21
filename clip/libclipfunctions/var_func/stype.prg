****************************
function stype(var)
	Local objLocal,nLastHandler,error,ret_val:="U",prom
	nLastHandler:=ERRORBLOCK({|x|Break(x)})

	Begin Sequence
		ret_val=Type(var)
	Recover Using error
	End Sequence
	if ret_val="U"
		Begin Sequence
			prom=&var
		Recover Using error
		End Sequence
		ret_val=valtype(prom)
	endif
	ERRORBLOCK(nLastHandler)
return ret_val
