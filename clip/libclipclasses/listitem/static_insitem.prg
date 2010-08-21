static func insItem(item, position, color, colorSel, sel)
    if item == NIL .or. position == NIL .or. !between(position, 1, ::itemCount)
	return 0
    endif
    color := iif(color==NIL, ::__colors[1], color)
    colorSel := iif(colorSel==NIL, ::__colors[2], colorSel)
    sel := iif(sel==NIL, .f., sel)
    ::itemCount ++
    asize(::item, ::itemCount)
    ains(::item, position)
    ::item[position]:item := item
    ::item[position]:color := color
    ::item[position]:scolor := colorSel
    ::item[position]:select := sel
    //::colorItem[position] := {color, colorSel}
    //::refresh() //why it need?
return 1
