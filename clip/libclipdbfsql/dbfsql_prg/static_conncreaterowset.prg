STATIC FUNCTION connCreateRowset(self,selectSQL,pars,insertSQL,deleteSQL,updateSQL,refreshSQL,idname,orders,gen_idSQL,nofetch,bEval,nEvery)
	LOCAL ors := map()
	LOCAL i

	ors:classname := "TROWSET"

	SQLLocWrite(self:conn,@selectSQL)
	SQLLocWrite(self:conn,@insertSQL)
	SQLLocWrite(self:conn,@deleteSQL)
	SQLLocWrite(self:conn,@updateSQL)
	SQLLocWrite(self:conn,@refreshSQL)

	ors:stmt := SQLPrepare(self:conn,selectSQL)

	ParLocWrite(self:conn,pars,self:RDBMS)

	ors:RDBMS      := self:RDBMS
	ors:connect    := self
	ors:conn       := self:conn
	ors:parameters := pars
	ors:idname     := idname
	ors:orders     := orders
	ors:curorder   := NIL
	ors:rowset     := NIL

	ors:selectSQL  := selectSQL
	ors:insertSQL  := insertSQL
	ors:deleteSQL  := deleteSQL
	ors:updateSQL  := updateSQL
	ors:refreshSQL := refreshSQL

	ors:Destroy        := @rowsetDestroy()
	ors:Read           := @rowsetRead()
	ors:Write          := @rowsetWrite()
	ors:BOF            := @rowsetBOF()
	ors:EOF            := @rowsetEOF()
	ors:Recno          := @rowsetRecno()
	ors:Lastrec        := @rowsetLastrec()
	ors:NFields        := @rowsetNFields()
	ors:FieldNo        := @rowsetFieldNo()
	ors:FieldName      := @rowsetFieldName()
	ors:FieldType      := @rowsetFieldType()
	ors:FieldTypeSQL   := @rowsetFieldTypeSQL()
	ors:FieldNullable  := @rowsetFieldNullable()
	ors:FieldUnsigned  := @rowsetFieldUnsigned()
	ors:FieldBinary    := @rowsetFieldBinary()
	ors:FieldLen       := @rowsetFieldLen()
	ors:FieldDec       := @rowsetFieldDec()
	ors:GetValue       := @rowsetGetValue()
	ors:SetValue       := @rowsetSetValue()
	ors:Append         := @rowsetAppend()
	ors:Delete         := @rowsetDelete()
	ors:GoTop          := @rowsetGoTop()
	ors:GoBottom       := @rowsetGoBottom()
	ors:Skip           := @rowsetSkip()
	ors:Goto           := @rowsetGoto()
	ors:ToClip         := @rowsetToClip()
	ors:FromClip       := @rowsetFromClip()
	ors:FieldBlock     := @rowsetFieldBlock()
	ors:Browse         := @rowsetBrowse()
	ors:Refresh        := @rowsetRefresh()
	ors:RefreshAll     := @rowsetRefresh()
	ors:RefreshCurrent := @rowsetRefreshCurrent()
	ors:CreateOrder    := @rowsetCreateOrder()
	ors:Seek           := @rowsetSeek()
	ors:SetOrder       := @rowsetSetOrder()
	ors:Fetched        := @rowsetFetched()
	ors:FetchAll       := @rowsetFetchAll()
	ors:KeyNo          := @rowsetKeyNo()

	ors:rowset := SQLCreateRowset(ors,ors:stmt,pars,idname,orders,gen_idSQL)

	IF nofetch == NIL .OR. !nofetch
		SQLFetch(ors:rowset,0,bEval,nEvery,ors)
	ENDIF

	IF orders != NIL
		SQLFillOrders(ors:rowset)
	ENDIF
	AADD(self:Rowsets,ors:rowset)

RETURN ors
