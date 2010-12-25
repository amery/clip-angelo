Function dispbox(top, left, bottom, right, xType,color)
	if !set(_SET_DISPBOX)
		return dispboxTerm(top, left, bottom, right, xType,color)
	else
		return dispboxSay(top, left, bottom, right, xType,color)
	endif
return
