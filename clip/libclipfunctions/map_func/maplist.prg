function mapList(cname)
	local i,n,m,ret:={}
	if empty(cname) .or. valtype(cname)==NIL
        	return map()
        endif
        cname=upper(cname)
        m=mapKeys(maps)
        for i=1 to len(m)
        	if maps[m[i]]:classname==cname
                	aadd(ret,maps[m[i]])
                endif
        next
return ret
