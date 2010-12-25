function htmlParserNew()
	local obj:=map()
	obj:error	:= ""
	obj:buffer	:= ""
	obj:tags	:= {}
	obj:put		:= @hp_put()
	obj:getTag	:= @hp_get()
	obj:get		:= @hp_get()
	obj:end		:= @hp_end()
	obj:empty	:= @hp_empty()
	obj:__parseBuf	:= @hp_parse()
	obj:__addText	:= @hp_text()
	obj:__parseTag	:= @hp_parseTag()
return obj
