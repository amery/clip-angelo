static function db_Get(row,col,var,length,color,pic,vblock,wblock,name)
local m, varname, block, getList:= {}
local nt,nl,nb,nr
	color := iif(color==NIL .or. empty(color), ::__colors[5], color)
	length := iif(length==NIL, len(var), length)
	nt := iif(row==NIL, ::lines+1, row)
	nl := iif(col==NIL, ::n_pos, col)
	nb := nt
	nr := nl+length
	name := iif(name==NIL, "", name)
	if var==NIL
		var := ""
	endif
	::check_line(nb)
	//asize(::inbuffer, ::lines)
	::__nbr ++
	varname := "get"+alltrim(str(::__nbr))
	block := {|_1| iif(_1==NIL, var,var:=_1)}
	pic := iif(pic == NIL, "@kS"+alltrim(str(length)), pic)
	m := map()
	m:objit := getNew(nt,nl,block,varname,pic,color,var,vblock,wblock)
	m:color := color
	m:number := ::__nbr
	m:nt := nt
	m:nl := nl
	m:nb := nb
	m:nr := nr
	m:type := 2
	m:name := name
	aadd(::inbuffer[nt], m)
	asort(::inbuffer[nt],,,{|x, y| x:nl < y:nl})
	::n_Line := nb
	::n_Pos  := nr
return ::__nbr
