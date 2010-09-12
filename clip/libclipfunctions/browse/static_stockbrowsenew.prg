STATIC FUNCTION StockBrowseNew(nTop, nLeft, nBottom, nRight)
    LOCAL browse
    LOCAL n, column
    browse := TBrowseDB(nTop, nLeft, nBottom, nRight)
    FOR n := 1 TO FCount()
	column := TBColumnNew(Field(n), FieldWBlock(Field(n), Select()))
	browse:addColumn(column)
    NEXT
RETURN browse
