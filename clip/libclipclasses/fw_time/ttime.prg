function TTime()

	local obj	:= map()
        obj:classname	:= "TTIME"
        obj:nTime	:= 0
        obj:new		:= @tt_new()
        obj:system	:= @tt_system()
        obj:addHours	:= @tt_addHours()
        obj:addMinutes	:= @tt_addMinutes()
        obj:addSeconds	:= @tt_addSeconds()
        obj:cAdd	:= @tt_add()
        obj:Add		:= @tt_add()
        obj:cSub	:= @tt_sub()
        obj:Sub		:= @tt_sub()
        obj:cGetTime	:= @tt_getTime()
        obj:getTime	:= @tt_getTime()
        obj:sysChange	:= @tt_sysChange()

return obj
