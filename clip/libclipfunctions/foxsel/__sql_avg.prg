function __sql_avg(self,group_id,value)
	local i
        if valtype(self)!="O" .or. group_id==NIL .or. group_id<=0
        	return 0.00
        endif
        if len(self:count)<group_id
               	for i=len(self:count) to group_id
                	aadd(self:count,0.00)
                	aadd(self:sum,0.00)
                next
        endif
	if valType(value)=="N" .and. self:enable
        	self:count[group_id]++
        	self:sum[group_id]+=value
        endif
return self:sum[group_id]/self:count[group_id]
