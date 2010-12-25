function mapUnRegister(m)
        local map_ident, cname, ret
        if valtype(m)=="N"
        	map_ident=m
        endif
        if valtype(m)=="O"
               if "MAP_IDENT" $ m
                   map_ident:=m:map_ident
               endif
        endif
        if map_ident==NIL
        	return .f.
        endif
        cname:=maps[map_ident]:className
        adel(maps,map_ident)
        if empty(cname)
        	return .f.
        endif
        if cname $ mapsList
             ret:=ascan(mapsList[cname],{|x|x==map_ident})
             if ret!=0
                adel(mapsList[cname],ret)
                asize(mapsList[cname],len(mapsList[cname])-1)
             endif
             if len(mapsList[cname])==0
             	ret:=ascan(names,{|x|x==cName})
                if ret !=0
             	   adel(names,ret)
                   asize(names,len(names)-1)
                endif
             endif
        endif
return
