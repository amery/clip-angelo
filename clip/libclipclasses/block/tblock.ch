function TBlock()

	local obj       := map()
	obj:classname   := "FBLOCK"
	obj:cExpression := NIL
	obj:bBlock      := NIL
	obj:new         := @block_new()
	obj:eval        := @block_eval()
	obj:exec        := @block_eval()

return obj
