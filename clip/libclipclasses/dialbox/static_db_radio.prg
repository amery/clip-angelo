static function db_Radio(row,col,var,varname,caption,color,fblock,sblock, name)
local m, message, style, bitmaps, getList:= {}, block, varnm
local nt,nl,nb,nr
	color := iif(color==NIL .or. empty(color), ::__colors[9]+","+::__colors[10], color)
	caption := iif(caption==NIL, "", caption)
	nt := iif(row==NIL, ::lines+1, row)
	nl := iif(col==NIL, ::marginLeft, col)
	nb := nt
	nr := nl+2
	name := iif(name==NIL, "", name)
	::check_line(nb)
	//asize(::inbuffer, ::lines)
	if !empty(caption)
		::Text(caption, nt, nl+4,,, color)
		caption := ""
	endif
	::__nbr ++
	varnm := upper("radio"+varname)//alltrim(str(::__nbr))
	block := {|_1| iif(_1==NIL, var,var:=_1)}
	if !(varnm$::RadioItem)
		::RadioItem[varnm] := map()
		::RadioItem[varnm]:radio := radioGroup(nt, nl, nb, nr)
		::RadioItem[varnm]:getobj := getNew(nt,nl,block,varnm,,color,var)
		::RadioItem[varnm]:radio:sayBox := .f.
	endif
	::RadioItem[varnm]:radio:bottom := max(nb, ::RadioItem[varnm]:radio:bottom)
	::RadioItem[varnm]:radio:top := min(nt, ::RadioItem[varnm]:radio:top)
	if var!=NIL
		if valtype(var)=="L"
			var := iif(var, len(::RadioItem[varnm]:radio:__items)+1, 0)
		endif
		::RadioItem[varnm]:radio:value := var
	endif

	::RadioItem[varnm]:radio:addItem(radioButton(nt, nl, caption))
	if (var!=NIL) .and. (var == len(::RadioItem[varnm]:radio:__items))
		::RadioItem[varnm]:radio:__items[var]:setFocus()
		::RadioItem[varnm]:radio:__items[var]:buffer := .t.
		::RadioItem[varnm]:radio:value := var
	endif
	m := map()
	m:objit := ::RadioItem[varnm]:getobj
	m:objit:Control := ::RadioItem[varnm]:radio
	m:color := color
	m:number := ::__nbr
	m:nt := nt
	m:nl := nl
	m:nb := m:nt
	m:nr := nr
	m:type := 4
	m:name := name
	aadd(::inbuffer[nt], m)
	asort(::inbuffer[nt],,,{|x, y| x:nl < y:nl})
	::n_Line := nb
	::n_Pos  := nl+3
return ::__nbr
