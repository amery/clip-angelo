static function te_highLightAdd(line, color)
	if !between(line, 1, ::lines)
		return .f.
	endif
	if line$::highLightColor
		aadd(::highLightColor[line], color)
		::highLightColor[line] := asort(::highLightColor[line],,, {|x, y| x<y})
	else
		::highLightColor[line] := {}
		aadd(::highLightColor[line], color)
		::highLightColor[line] := asort(::highLightColor[line],,, {|x, y| x<y})
	endif
	::refresh()
return .t.
