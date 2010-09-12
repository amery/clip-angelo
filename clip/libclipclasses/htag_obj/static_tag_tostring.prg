static function tag_toString()
	local ret,i, arr
	ret:="<"+::tagName+" "
	if !empty(::other)
		ret+=alltrim(toString(::other))+' '
	endif
	arr := mapkeys(::fields)
	for i=1 to len(arr)
		if ::fields[arr[i]]==NIL
			ret+=hashname(arr[i])+' '
			loop
		else
			ret+=hashname(arr[i])+'='
		endif
		if valtype(::fields[arr[i]])=="N"
			ret+=alltrim(toString(::fields[arr[i]]))+' '
		else
			ret+='"'+alltrim(toString(::fields[arr[i]]))+'" '
		endif
	next
	ret:=left(ret,len(ret)-1)
	ret+=">"
return ret
