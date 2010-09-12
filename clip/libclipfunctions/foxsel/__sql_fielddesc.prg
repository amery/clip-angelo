function __sql_fieldDesc(self,xname)
	local f
        for f in self:fields
        	if f:xname == xname
                	return f
                endif
        next
return NIL
