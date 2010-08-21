function cobraQueryNew()
	local obj:=map()
	obj:classname	:= "COBRAQUERY"
	obj:id		:= (query_counter++)
	obj:command	:= ""
	obj:args	:= {}
	obj:answer	:= NIL
	obj:return	:= NIL
return obj
