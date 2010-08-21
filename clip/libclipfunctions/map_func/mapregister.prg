function mapRegister(className)
	local obj:=map()

        if empty(className) .or. valType(classname)!="C"
        	return obj
        endif
        if maps==NIL
        	maps:=map()
        endif
        if mapsList==NIL
        	mapsList:=MAP()
        endif
        className	:= upper(className)
        obj:className	:= className
        map_count++
        obj:map_ident	:= map_count
        maps[map_count]	:= obj
        obj:Destroy	:= @map_destroy()
        if !(className $ mapsList)
            aadd(names, className)
            mapsList[className]:={}
        endif
        aadd(mapsList[className],map_count)

return obj
