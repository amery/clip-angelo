static function sql_parse(sQuery)
	local i,j,k,x,s,ch1,ch2,m:={}
        local keyWords:={"from","into","to","noconsole","plain","nowait",;
        		 "where","group by","having","order by"}
        /*
        i:=1
        while at("  ",sQuery)!=0 .and. i<30
        	sQuery:=strtran(sQuery,"  "," ")
                i++
        enddo
        */
        sQuery:=strtran(sQuery,";&\n","")
        sQuery:=strtran(sQuery,"&\t"," ")
        sQuery:=alltrim(sQuery)
        if lower(substr(sQuery,1,7))=="select "
        	sQuery:=alltrim(substr(sQuery,8))
        else
        	fx_sql_error("bad expression in SELECT:"+s,__LINE__)
        endif
        s:=lower(sQuery)
        for i=len(keyWords) to 1 step -1
        	k:=1
        	while .t.
        		j:=at(keyWords[i],s,k)
                	if j<=0
                        	exit
                        endif
                	ch1:=substr(s,j-1,1)
                        ch2:=substr(s,j+len(keyWords[i]),1)
                        if ch1==" " .and. (ch2==" " .or. j+len(keyWords[i]) > len(s) )
        			aadd(m,{keyWords[i],j})
                                exit
                        else
                        	k++
                        endif
                enddo
        next
        if len(m)==0
        	fx_sql_error("bad expression in SELECT:"+s,__LINE__)
        endif
        asort(m,,,{|x,y|x[2]<y[2]})
        j:=m[1][2]; s:=m[1][1]
        ::fields:=alltrim(substr(sQuery,1,j-1))
        for i=2 to len(m)+1
            x:=iif(i>len(m),len(sQuery)+2,m[i][2])
            do case
            	case s=="from"
                	::from:=substr(sQuery,j+5,x-j-6)
            	case s=="where"
                	::where:=substr(sQuery,j+6,x-j-7)
            	case s=="order by"
                	::order:=substr(sQuery,j+9,x-j-10)
            	case s=="group by"
                	::group:=substr(sQuery,j+9,x-j-10)
            	case s=="to"
                	::to:=substr(sQuery,j+3,x-j-4)
            	case s=="noconsole"
                	::console:=.f.
            	case s=="having"
                	::having:=substr(sQuery,j+7,x-j-8)
            	case s=="nowait"
                	::wait:=.f.
            	case s=="plain"
                	::plain:=.t.
            	case s=="into"
                	::into:=.t.
                	::to:=substr(sQuery,j+5,x-j-4)
            endcase
            if i>len(m)
            	exit
            endif
            j:=m[i][2]; s:=m[i][1]
        next
        if ::into
        	::plain:=.f.
        endif
return
