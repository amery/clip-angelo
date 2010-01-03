static func mi_status(self,member,id,newValue)
	local ret
        if valtype(self:data)=="O" .and. self:data:classname=="POPUPMENU"
        	return self:data:_status(member,id,NewValue)
        endif
        if id==self:id .and. member $ self
        	ret:=self[member]
                if newValue!=NIL
                	self[member] := newValue
                endif
        endif
return ret
