STATIC PROCEDURE FancyColors(browse)
    LOCAL n, column
    LOCAL xValue
    browse:colorSpec := setcolor()
    if empty(browse:colorSpec)
	 browse:colorSpec := "N/W, N/BG, B/W, B/BG, B/W, B/BG, R/W, B/R"
    else
	 browse:colorSpec := setcolor()+", N/W, N/BG, B/W, B/BG, B/W, B/BG, R/W, B/R"
    endif
    FOR n := 1 TO browse:colCount
	column := browse:getColumn(n)
	xValue := EVAL(column:block)

	IF VALTYPE(xValue) != "N"
	    column:defColor := {3, 4}
	ELSE
	    column:colorBlock := {|x| if( x < 0, {7, 8}, {5, 6} )}
	    column:defColor := {7, 8}
	ENDIF
    NEXT
RETURN
