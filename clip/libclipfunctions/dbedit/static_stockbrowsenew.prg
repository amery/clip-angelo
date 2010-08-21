STATIC FUNCTION StockBrowseNew( browse, fields, headers, footers, msay, hSep, cSep, fSep )
    LOCAL n, column, cType, strhead, strfoot, strpict
    local __cname
    if empty( fields )
	fields:={}
	asize(fields,fcount())
	afields(fields)
    endif
    if headers == NIL
	  headers:={}
    endif
    if footers == NIL
	  footers:={}
    endif
    if hSep == NIL
	  hSep:={}
    endif
    if cSep == NIL
	  cSep:={}
    endif
    if fSep == NIL
	  fSep:={}
    endif
    if mSay == NIL
	  mSay:={}
    endif
    *************
    if valtype(headers)=="C"
       strhead=headers
       headers:={}
       asize(headers,len(fields))
       afill(headers,strhead)
    endif
    if valtype(headers)=="A"
       for n = len(headers)+1 to len(fields)
	   aadd(headers,fields[n])
       next
    endif
    if valtype(footers)=="C"
       strfoot=footers
       footers:={}
       asize(footers,len(fields))
       afill(footers,strfoot)
    endif
    if valtype(footers)=="A"
       for n = len(footers)+1 to len(fields)
	   aadd(footers,"")
       next
    endif
    if valtype(hSep)=="C"
       strhead=hSep
       hSep:={}
       asize(hSep,len(fields))
       afill(hSep,strhead)
    endif
    if valtype(hSep)=="A"
       for n = len(hSep)+1 to len(fields)
	   aadd(hSep,DE_HEADSEP)
       next
    endif
    if valtype(cSep)=="C"
       strhead=cSep
       cSep:={}
       asize(cSep,len(fields))
       afill(cSep,strhead)
    endif
    if valtype(cSep)=="A"
       for n = len(cSep)+1 to len(fields)
	   aadd(cSep,DE_COLSEP)
       next
    endif
    if valtype(fSep)=="C"
       strhead=fSep
       fSep:={}
       asize(fSep,len(fields))
       afill(fSep,strhead)
    endif
    if valtype(fSep)=="A"
       for n = len(fSep)+1 to len(fields)
	   aadd(fSep,'')
       next
    endif
    if valtype(msay)=="C"
	strpict=msay
	mSay:={}
	asize(msay,len(fields))
	afill(msay,strpict)
    elseif valtype(msay) != "A"
	msay:=array(len(fields))
    endif
    asize(msay,len(fields))
    FOR n := 1 TO len(fields)
	if empty(fields[n])
		loop
	endif
	__cname:="{|| "+fields[n]+"}"
	__cname:=&__cname
	//column := TBColumnNew(Field(n), FieldWBlock(Field(n), Select()))
	column := TBColumnNew(fields[n], __cname)
	column:heading:=headers[n]
	column:footing:=footers[n]
	column:picture:=msay[n]
	column:headSep:=hsep[n]
	column:colSep:=csep[n]
	column:footSep:=fsep[n]
	browse:addColumn(column)
    next
RETURN NIL
