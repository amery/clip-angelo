static func GetReadVar(get)
	local name := Upper(get:name)
	local i
	if ( get:subscript <> NIL )
		for i := 1 to len(get:subscript)
			name += "[" + ltrim(str(get:subscript[i])) + "]"
		next
	end
return (name)
