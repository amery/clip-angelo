***************************************
* pop key
***************************************
function fx_popKeys(lAll)
	local keys
        if len(stack_setKeys) > 0
        	keys := atail(stack_setKeys)
                restSetKey(keys)
                asize(stack_setKeys,len(stack_setKeys)-1)
        endif
return keys
