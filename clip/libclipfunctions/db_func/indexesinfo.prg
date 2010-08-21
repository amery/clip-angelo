// return structure with info for all aliases
function indexesInfo(sel)
	local ret,i,oldsel:=select()
	if sel!=NIL
			select(sel)
		endif
		ret:={}
		i:=1
		do while .t.
			if empty(ordname(i))
				exit
			endif
			aadd(ret,{ordbagname(i),ordname(i),indexkey(i)})
			i++
		enddo
		select(sel)
return ret
