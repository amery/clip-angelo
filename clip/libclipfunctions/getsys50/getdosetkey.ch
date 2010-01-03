proc GetDoSetKey(keyBlock, get)
	local saveUpdated
	if ( get:changed )
		get:Assign()
		status:Updated := .t.
	end
	saveUpdated := status:Updated
	Eval(keyBlock, status:ReadProcName, status:ReadProcLine, ReadVar())
	ShowScoreboard()
	get:UpdateBuffer()
	status:Updated := saveUpdated
	if ( status:KillRead )
	     get:exitState := GE_ESCAPE    // О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫ О©╫О©╫ ReadModal()
	end
return
