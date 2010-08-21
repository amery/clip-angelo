static function sql_append()
	local i,j,m,tmp:={}
	if len(::groupBlocks)>0
        	// get data for group fields
        	for i in ::groupBlocks
                	aadd(tmp,eval(i))
                next
                // search group key and number of record
                ::__bufferLine:=0
                for i=1 to len(::groupKeys)
                	m:=::groupKeys[i]
                 	for j=1 to len(tmp)
                        	if ! (m[j]==tmp[j])
                                	exit
                                endif
                        next
                        if j>len(tmp)
                        	::__bufferLine:=i
                                exit
                        endif
                next
                if ::__bufferLine==0
                	aadd(::groupKeys,tmp)
                        aadd(::buffer,map())
                        ::__bufferLine:=len(::groupKeys)
                endif
        else
        	if ::gFunctions
                	::__bufferLine:=1
                        if len(::groupKeys)==0
                        	aadd(::groupKeys,.t.)
                                aadd(::buffer,map() )
                        endif
                else
                	//outlog(__FILE__,__LINE__,len(::groupKeys))//,::groupKeys)
                        aadd(::groupKeys,.t.)
                	//outlog(__FILE__,__LINE__,len(::groupKeys),len(::buffer))//,::groupKeys)
                        aadd(::buffer,map() )
                	//outlog(__FILE__,__LINE__,len(::buffer))//,::groupKeys)
                	::__bufferLine:=len(::groupKeys)
                	//outlog(__FILE__,__LINE__,len(::__bufferLine))//,::groupKeys)
                endif
        endif
        //outlog(__FILE__,__LINE__,len(::__bufferLine))//,::groupKeys)
return
