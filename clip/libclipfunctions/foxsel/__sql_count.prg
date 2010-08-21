function __sql_count(self,group_id,value)
	local i
        if valtype(self)!="O" .or. group_id==NIL .or. group_id<=0
        	return 0.00
        endif
        if len(self:count)<group_id
               	for i=len(self:count) to group_id
                	aadd(self:count,0.00)
                next
        endif
	//if valType(value)=="N" .and. self:enable
	if !empty(value) .and. self:enable
        	self:count[group_id]++
        endif
return self:count[group_id]
