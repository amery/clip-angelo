*********************************************
* internal function for SQL query
*********************************************
function __sql_sum(self,group_id,value)
	local i
        if valtype(self)!="O" .or. group_id==NIL .or. group_id<=0
        	return 0.00
        endif
        if len(self:sum)<group_id
               	for i=len(self:sum) to group_id
                	aadd(self:sum,0.00)
                next
        endif
	if valType(value)=="N" .and. self:enable
        	self:sum[group_id]+=value
        endif
return self:sum[group_id]
