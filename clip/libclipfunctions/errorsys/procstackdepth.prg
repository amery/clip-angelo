function procStackDepth()
	local i := 1
	while ( !Empty(ProcName(i)) )
		i++
	end
return i
