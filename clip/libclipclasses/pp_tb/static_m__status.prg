static function m__status(self,member,id,newValue)
	local o,ret
	for o in self:__items
		if valtype(o)=="O"
			ret := o:_status(member,id,newValue)
		endif
		if ret!=NIL
			exit
		endif
	next
return ret
