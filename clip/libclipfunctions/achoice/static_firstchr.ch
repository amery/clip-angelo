static func firstChr(chrKey,bb)
	local ccc,retVal
	ccc:=upper(chr(chrKey))
	retVal:=ascan(__ac_data:massP,{|elem|iif( elem != NIL .and. upper(substr(ltrim(elem),1,1))==ccc,.t.,.f.)},__ac_data:nRowmass+1)
	if retVal==0 .and. __ac_data:nRowMass >= 1
		retVal:=ascan(__ac_data:massP,{|elem| elem != NIL .and. iif(upper(substr(ltrim(elem),1,1))==ccc,.t.,.f.)},1)
	endif
	if retVal!=0 .and. __ac_data:_lmass[retVal]
		__ac_data:nrowMass:=retVal
		if retVal < __ac_data:nB-__ac_data:nT
			bb:rowPos := retVal
		endif
	endif
return 3
