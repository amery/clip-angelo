static function db_Text(txt, nt, nl, nb, nr, color, name)
local i, len, m
	nt := iif(nt==NIL, ::lines+1, nt)
	nl := iif(nl==NIL, ::marginLeft, nl)
	nb := iif(nb==NIL, nt, nb)
	nr := iif(nr==NIL, nl+len(txt)+1, nr)
	color := iif(color==NIL .or. empty(color), ::colorSpec, color)
	name := iif(name==NIL, "", name)
	len := nr-nl+1
	::check_line(nb)
	//asize(::inbuffer, ::lines)
	::__nbr++

	for i=nt to nb
		m := map()
		m:color := color
		m:txt := substr(txt, 1, len)
		m:number := ::__nbr
		m:nt := i
		m:nl := nl
		m:nb := i
		m:nr := nr
		m:type := 0
		m:name := name
		aadd(::inbuffer[i], m)
		txt := substr(txt, len+1)
		asort(::inbuffer[i],,,{|x, y| x:nl < y:nl})
	next
	::n_Line := nb
	::n_Pos  := nr-1
return ::__nbr
