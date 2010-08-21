static  function stack_pop()
	local var_data:=NIL
        if len(::data)>0
	   var_data:=atail(::data)
           asize(::data,len(::data)-1)
        endif
return var_data
