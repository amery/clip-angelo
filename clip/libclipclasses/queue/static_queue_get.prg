static  function queue_get()
	local var_data:=NIL
        if len(::data)>0
	   var_data:=::data[1]
           adel(::data,1)
           asize(::data,len(::data)-1)
        endif
return var_data
