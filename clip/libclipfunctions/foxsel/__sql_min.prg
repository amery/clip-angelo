function __sql_min(self,group_id,value)
	local i
        if valtype(self)!="O" .or. group_id==NIL .or. group_id<=0
        	return 0.00
        endif
        if len(self:min)<group_id
               	for i=len(self:min) to group_id
                	aadd(self:min,infinity())
                next
        endif
	if valType(value)=="N" .and. self:enable
        	self:min[group_id]=min(self:min[group_id],value)
        endif
return self:min[group_id]
