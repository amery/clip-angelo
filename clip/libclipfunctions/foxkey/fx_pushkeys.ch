***************************************
* push key
***************************************
function fx_pushKeys(lClear)
	local i, keys := saveSetKey()
        aadd(stack_setKeys,keys)
        if lClear
        	for i=MIN_KEY_CODE to MAX_KEY_CODE
        		setkey(i, NIL)
        	next
        endif
return keys
