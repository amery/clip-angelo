static func __getcolor(self,num)
	local len,ret := "W/N"
	len := Len(self:__colors)
	if len>1
		ret := atail(self:__colors)
	endif
	if num>0 .and. num<=len
		ret := self:__colors[num]
	elseif num>1
		ret := nToColor(invertAttr(ret))
	endif
return ret
