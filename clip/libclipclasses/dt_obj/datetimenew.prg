function dateTimeNew(p1,p2,p3,p4,p5,p6,p7)
	local obj:=map()

	obj:classname	:= "DATETIME"
	obj:data	:= dt_create(p1,p2,p3,p4,p5,p6,p7)

	_recover_datetime(obj)

return obj
