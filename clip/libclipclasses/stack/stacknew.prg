function stackNew()
	local obj:=map()

        obj:data:={}

	obj:push:=@stack_push()
	obj:pop:=@stack_pop()
	obj:len:=@stack_len()
	obj:free:=@stack_free()

return obj
