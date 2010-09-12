static function db_Check(row,col,var,caption,color,fblock,sblock, name)
local m, message, style, bitmaps, getList:= {}, block, varname
local nt,nl,nb,nr
	color := iif(color==NIL .or. empty(color), ::__colors[7]+","+::__colors[8], color)
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
	varname := "check"+alltrim(str(::__nbr))
	block := {|_1| iif(_1==NIL, var,var:=_1)}
	m := map()
	m:objit := getNew(nt,nl,block,varname,,color,var)
	m:objit:Control := _checkbox_(var,caption,message,color,fblock,sblock,style,bitmaps,nt,nl)
	m:color := color
	m:number := ::__nbr
	m:nt := nt
	m:nl := nl
	m:nb := nb
	m:nr := nr
	m:type := 3
	m:name := name
	aadd(::inbuffer[nt], m)
	asort(::inbuffer[nt],,,{|x, y| x:nl < y:nl})
	::n_Line := nb
	::n_Pos  := nl+3
return ::__nbr
