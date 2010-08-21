function queueNew()
	local obj:=map()

        obj:data:={}

	obj:put:=@queue_put()
	obj:get:=@queue_get()
	obj:head:=@queue_head()
	obj:tail:=@queue_tail()
	obj:len:=@queue_len()
	obj:free:=@queue_free()
	obj:empty:=@queue_empty()

return obj
