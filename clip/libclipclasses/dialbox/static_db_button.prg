static function db_Button(txt, fnc, nt, nl, nb, nr, color, name, info)
local i, len, m
	if valtype(fnc) == "O"
		name := iif(name==NIL, "", name)
	else
		nt := iif(nt==NIL, ::lines+1, nt)
		nl := iif(nl==NIL, ::n_Pos, nl)
		nb := iif(nb==NIL, nt, nb)
		nr := iif(nr==NIL, nl+len(txt)+1, nr)
		color := iif(color==NIL .or. empty(color), ::__colors[3]+","+::__colors[4], color)
		name := iif(name==NIL, "", name)
		info := iif(info==NIL, "", info)
	endif
	len := nr-nl+1
	::check_line(nb)
	//asize(::inbuffer, ::lines)
	::__nbr++
	for i=nt to nb
		m := map()
		m:txt := substr(txt, 1, len)
		m:color := color
		m:info := info
		m:number := ::__nbr
		m:nt := i
		m:nl := nl
		m:nb := i
		m:nr := nr
		m:type := 1
		m:fnc := fnc
		m:name := name
		aadd(::inbuffer[i], m)
		txt := substr(txt, len+1)
		asort(::inbuffer[i],,,{|x, y| x:nl < y:nl})
	next
	::n_Line := nb
	::n_Pos  := nr-1
return ::__nbr
