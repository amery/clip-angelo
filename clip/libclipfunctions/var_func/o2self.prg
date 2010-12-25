**************************************
function o2self(self,Obj)
	local k

	if valtype(Obj)!="O"; return nil; endif

	for k in Obj keys
		if valtype(Obj[k])=="B"
			self[k] := Obj[k]
		else
			self[k] := @Obj[k]
		endif
	next
return self
