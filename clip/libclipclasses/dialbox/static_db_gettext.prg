static function db_GetText(top,left,bottom,right,var,color,pic,vblock,wblock,name)
local m, varname, block, getList:= {}, item, arr, length, color_item
local i,nt,nl,nb,nr
	color := iif(color==NIL .or. empty(color), ::__colors[5]+","+::__colors[6], color)
	color_item := substr(color, at(",", color)+1)
	color_item := iif(empty(color_item), ::__colors[6], color_item)
	nt := iif(top==NIL, ::lines+1, top)
	nl := iif(left==NIL, ::marginLeft, left)
	nb := iif(bottom==NIL, ::nBottom-1, bottom)
	nr := iif(right==NIL, ::nRight-1, right)
	name := iif(name==NIL, "", name)
	if var==NIL
		var := ""
	endif
	length := nr-nl+1
	::check_line(nb)
	//asize(::inbuffer, ::lines)
	::__nbr ++
	varname := "getTxt"+alltrim(str(::__nbr))
	block := {|_1| iif(_1==NIL, var,var:=_1)}
	item := TEXTGETNEW(top,left,bottom,right,block,varname,color_item,var,vblock,wblock)
	arr := split(var, "&\n")
	for i=1 to nb-nt+1
		m := map()
		m:objit := item
		m:color := color
		m:number := ::__nbr
		m:nt := nt+i-1
		m:nl := nl
		m:nb := m:nt
		m:nr := nr
		m:type := 5
		m:name := name
		aadd(::inbuffer[nt+i-1], m)
		asort(::inbuffer[nt+i-1],,,{|x, y| x:nl < y:nl})
	next
	::n_Line := nb
	::n_Pos  := nr-1
return ::__nbr
