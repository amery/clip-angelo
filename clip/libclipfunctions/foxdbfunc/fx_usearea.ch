function fx_useArea(db,alias,rdd,in,again,new,  ;
      	     	   share, ro, noUpdate, desc,	;
                   ilist,xOrder,cTag,xOf )

        local i,old_rdd:=RddSetDefault(rdd)

	if new
        	select 0
        endif
        if in != NIL
        	select(in)
        endif
        if alltrim(db)=="?"
        	db:= filedialog(,"*.dbf")
        endif
        if empty(db)
        	rddSetDefault(old_rdd)
        	return
        endif
        if len(ilist)==1 .and. alltrim(ilist[1])=="?"
        	ilist[1]:= filedialog(,"*"+indexExt())
        endif
        dbUseArea( NIL , rdd, db, alias, share, ro .or. noUpdate)
        for i=1 to len(iList)
      		ordListAdd( iList[i] )
        next
        ordSetFocus(xOrder,cTag)
        ordSetFocus(xOf,cTag)
        if desc
        	ordDescend()
        endif
return
