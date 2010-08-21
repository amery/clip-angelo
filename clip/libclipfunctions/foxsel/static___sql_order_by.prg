static function __sql_order_by(orderInfo,x1,x2)
	local i,c
        for i in orderInfo
        	c:=i:column
		if x1[c]==x2[c]
                else
                	if i:direction==SQL_DIRECTION_ASC
                        	return x1[c]<x2[c]
                        else
                        	return x1[c]>x2[c]
                        endif
                endif
        next
return .f.
