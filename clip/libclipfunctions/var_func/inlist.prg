**************************************
function inlist
	local i,j,p,ret:=.f.
	p=param(1)
	j=pcount()
	for i=2 to j
		if p==param(i)
			ret=.t.
			exit
		endif
	next
return ret
