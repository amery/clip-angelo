***************************************
* on key label <x> do <proc>
***************************************
function fx_setkey(cKey,bBlock)
	local nKey := fx_keyLabel2Num(cKey)
        if nKey != 0
      		SetKey( nKey, bBlock )
        endif
return nKey
