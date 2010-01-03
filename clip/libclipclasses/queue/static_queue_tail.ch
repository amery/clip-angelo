static  function queue_tail()
	local var_data:=NIL
        if len(::data)>0
	   var_data:=atail(::data)
        endif
return var_data
