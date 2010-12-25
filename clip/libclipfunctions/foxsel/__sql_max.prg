function __sql_max(self,group_id,value)
	local i
        if valtype(self)!="O" .or. group_id==NIL .or. group_id<=0
        	return 0.00
        endif
        if len(self:max)<group_id
               	for i=len(self:max) to group_id
                	aadd(self:max,0.00-infinity())
                next
        endif
	if valType(value)=="N" .and. self:enable
        	self:max[group_id]=max(self:max[group_id],value)
        endif
return self:max[group_id]
