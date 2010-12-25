static function sql_parseFields()
	local i,j,k,x,y, s, m, fname,aname,nAlias
        local hdb,xname,xnames:=map()
        local table
        if valtype(::fields)!="C"
        	return
        endif
        s:=alltrim(::fields); ::fields:={}
        if upper(substr(s,1,3))=="ALL"
        	::all:=.t.
                s:=alltrim(substr(s,4))
        endif
        if upper(substr(s,1,8))=="DISTINCT"
        	::all:=.f.
                s:=alltrim(substr(s,9))
        endif
        m:=split(s,",")
        for i=1 to len(m)
        	s:=m[i]
                while cscount("(",s) != cscount(")",s)
                	i++
                        if i<=len(m)
                		s+=","+ m[i]
                        else
                        	exit
                        endif
                enddo
                s := alltrim(s)
        	xname:=""; fname:=""; aname:=""
                if empty(s)
                	loop
                endif
                if "(" $ s  // call functions
                       	::functions:=.t.
                        s:=strtran(s,".",":")
                endif
        	j:=at(" AS ",upper(s))
        	if j>0
        		xname:=alltrim(substr(s,j+4))
                	s:=alltrim(substr(s,1,j))
        	endif
                if substr(s,1,1) == "{"
                	s:='dtoc("'+substr(s,2,len(s)-2)+'")'
                endif
                if isdigit(s)
                	j:=0
                else
        		j:=at(".",s)
                endif
        	if j>0
        		aname:=upper(alltrim(substr(s,1,j-1)))
                	fname:=alltrim(substr(s,j+1))
        	else
        		fname:=alltrim(s)
        	endif
                if !empty(aname)
                       	nAlias:=ascan(::from,{|x|x:alias==aname})
                        if nAlias<=0
        			fx_sql_error("Alias "+aname+" not found",__LINE__)
                        endif
                else
                	nAlias := 0
                endif
                if fname=="*"
                	if !empty(aname)
                        	y := { ::from[nAlias] }
                        else
                        	y := ::from
                        endif
        		for table in y
				x:=table:dbStruct
                        	for j=1 to len(x)
                        		xname:=x[j][1]
                			if ( xname $ xnames ) .and. !::allFields
                   				loop
                			endif
                			if ( xname $ xnames )
                				xnames[xname][1]++
                			else
                				xnames[xname]:={1,0}
                			endif
                                        k:=map()
                                        k:xname:=xname
                                        k:fname:=xname
                                        k:aname:=table:alias
                                        k:len  :=x[j][3]
                                        k:dec  :=x[j][4]
                                        k:order:=len(::fields)+1
                        		aadd(::fields,k)
                        	next
        		next
                        loop
                endif
                xname:=upper(xname)
        	if empty(xname)
        		xname:=upper(fname)
        	endif
        	if ( xname $ xnames ) .and. !::allFields
           		loop
        	endif
        	if ( xname $ xnames )
        		xnames[xname][1]++
        	else
        		xnames[xname]:={1,0}
        	endif
                fname:=upper(fname)
                //aname:=upper(aname)
                k:=map()
                k:xname:=upper(xname)
                k:fname:=fname
                k:aname:=aname
                k:order:=len(::fields)+1
                k:len  :=0
                k:dec  :=0
                aadd(::fields,k)
        next
        for i in ::fields      // ������������ ����
        	xname:=i:xname
                if empty(i:aname) // default table
                	i:aname:=::from[1]:alias
                endif
                if "(" $ xname
                	loop
                endif
                if xnames[xname][1]>1
                      xname:=substr(xname,1,8)
             	      xname+="_"+chr(64+xnames[i:xname][2]++)
                endif
                i:xname:=xname
        next
        if !( "EXP" $ xnames )
        	xnames:exp:={1,0}
        endif
        for i in ::fields // check tables, fields, functions
                aname:=i:aname
                table:=ascan(::from,{|x|x:alias==aname})
                if table<=0
                	loop
                endif
                hdb:=::from[table]:hDB
                if hDB==NIL // error open this alias
                	loop
                endif
                xname:=i:xname
                fname:=i:fname
                m:=::from[table]:dbstruct
                j:=ascan(m,{|x|x[1]==upper(fname)})
                if j>0
                	// code block for get field data
                	s:="{|| __sql_GetValue("+;
                		alltrim(str(hDB))+;
                        	",'"+fname+"','"+xname+"')}"
                	i:len  :=m[j][3]
                	i:dec  :=m[j][4]
                	i:sblock:=s
                	i:block:=&s
                        loop
                endif

                if Upper(xname)==upper(fname)
                	i:xname:="EXP_"+chr(64+xnames:exp[2]++) //fields[i][1]
                else
                       	i:xname:=xname
                endif
                if (x:=at("(",fname))>0         // call functions
                	s:=Upper(substr(fname,1,x-1))  // function name
                        fname:=substr(fname,x+1, len(fname)-x-1)
                	j:=ascan(m,{|x|x[1]==upper(fname)})
                        if j>0
                		i:len  :=m[j][3]
                		i:dec  :=m[j][4]
                		fname:="__sql_GetValue("+;
                			alltrim(str(hDB))+;
                			",'"+fname+"','"+xname+"')"
                        endif
                        do case
                        	case s=="COUNT"
                                	if "*" $ fname
                                		::gFunctions:=.t.
                                		fname:="__sql_count(x,y,1)"
                                                i:len:=10
                                        	i:count:={}
                                        	i:enable:=.t.
                				s:="{|x,y| "+fname+"}"
                                                ::groupBlocks:={{||"count"}}
                                        else
                                		::gFunctions:=.t.
                                		fname:="__sql_count(x,y,"+fname+")"
                                                i:len:=10
                                        	i:count:={}
                                        	i:enable:=.t.
                				s:="{|x,y| "+fname+"}"
                				s:="{|x,y| "+fname+"}"
                                        endif
                        	case s=="SUM"
                                	::gFunctions:=.t.
                                	fname:="__sql_sum(x,y,"+fname+")"
                                        i:len:=max(i:len,16)
                                        i:sum:={}
                                        i:enable:=.t.
                			s:="{|x,y| "+fname+"}"
                        	case s=="MAX"
                                	::gFunctions:=.t.
                                	fname:="__sql_max(x,y,"+fname+")"
                                        i:len:=max(i:len,16)
                                        i:max:={}  //0-infinity()
                                        i:enable:=.t.
                			s:="{|x,y| "+fname+"}"
                        	case s=="MIN"
                                	::gFunctions:=.t.
                                	fname:="__sql_min(x,y,"+fname+")"
                                        i:len:=max(i:len,16)
                                        i:min:={} //infinity()
                                        i:enable:=.t.
                			s:="{|x,y| "+fname+"}"
                        	case s=="AVG"
                                	::gFunctions:=.t.
                                	fname:="__sql_avg(x,y,"+fname+")"
                                        i:len:=max(i:len,16)
                                        i:enable:=.t.
                                        i:count:={}
                                        i:sum:={}
                			s:="{|x,y| "+fname+"}"
                                otherwise
                                	fname:=s+"("+fname+")"
                			s:="{|| "+fname+"}"
                        endcase
                else 				// constant
                	s:="{|| "+fname+"}"
                endif
                i:sblock:=s
                i:block:=&s
        next
return
