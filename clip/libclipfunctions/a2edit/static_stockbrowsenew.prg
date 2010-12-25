STATIC FUNCTION StockBrowseNew( browse, len_data, headers, footers )
    LOCAL n, column, cType, strhead, strfoot
    local __cname
    if empty(headers)
	  headers:={}
    endif
    if empty(footers)
	  footers:={}
    endif
    if valtype(headers)=="C"
       strhead=headers
       headers:={}
       asize(headers,len_data)
       afill(headers,strhead)
    endif
    if valtype(headers)=="A"
       for n = len(headers)+1 to len_data
	   aadd(headers,str(n))
       next
    endif
    if valtype(footers)=="C"
       strfoot=footers
       footers:={}
       asize(footers,len_data)
       afill(footers,strfoot)
    endif
    if valtype(footers)=="A"
       for n = len(footers)+1 to len_data
	   aadd(footers,"")
       next
    endif
    FOR n := 1 TO len_data
	__cname:="{|_1| __a2_edit_get_a2_data("+alltrim(str(n))+",_1)}"
	__cname:=&__cname
	column := TBColumnNew("__a2_data["+alltrim(str(n))+"]", __cname)
	column:heading:=headers[n]
	column:footing:=footers[n]
	browse:addColumn(column)
    next
RETURN NIL
