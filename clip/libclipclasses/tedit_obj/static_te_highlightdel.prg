static function te_highLightDel(line, color)
local ret := .f., i
	if line$::highLightColor
		for i=1 to len(::highLightColor[line])
			if ::highLightColor[line][i] == color
				ret := adel(::highLightColor[line], i)
				asize(::highLightColor[line], len(::highLightColor[line])-1)
				exit
			endif
		next
		if empty(::highLightColor[line])
			ret := adel(::highLightColor, line)
		endif
	endif
	::refresh()
return ret
