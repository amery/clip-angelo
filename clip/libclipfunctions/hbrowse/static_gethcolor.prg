static function getHcolor(oldcolor, newcolor)
	newcolor := upper(newcolor)
	if newcolor$::htmlColors
		return (::htmlColors[newcolor]+substr(oldcolor, at("/", oldcolor)))
	endif
return oldcolor
