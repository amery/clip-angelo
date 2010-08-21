STATIC PROCEDURE FancyColors(browse)
    LOCAL n, column
    LOCAL xValue
    browse:colorSpec := setcolor()
    if empty(browse:colorSpec)
	 browse:colorSpec := DE_COLOR
    else
	 browse:colorSpec := setcolor()+","+DE_COLOR
    endif
    FOR n := 1 TO browse:colCount
	column := browse:getColumn(n)
	xValue := EVAL(column:block)

	column:defColor := {1, 2}
    NEXT
RETURN
