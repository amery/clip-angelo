static func addItem(item, color, colorSel)
local m := map()
    if item == NIL
	return 0
    endif
    color := iif(color==NIL, ::__colors[1], color)
    colorSel := iif(colorSel==NIL, ::__colors[2], colorSel)
    m:item := item
    m:color := color
    m:scolor := colorSel
    m:select := .f.
    aadd(::item, m)
    //aadd(::colorItem, {color, colorSel})
    ::itemCount ++
    m := ""
return 1
