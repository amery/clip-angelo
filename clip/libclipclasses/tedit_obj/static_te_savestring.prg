static function te_saveString()
local i, n, str:="", ts, hch, ch

  ts := ::TabSize
  if ::charset == NIL
	if ::tabPack
		aeval(::edbuffer, {|item| ;
			str:=str+tabpack(padr(item, len(item)-len(ltrim(item))), ts)+ltrim(item)+"&\n"})
		/*
		for i=1 to ::lines
			n := len(::edbuffer[i])-len(ltrim(::edbuffer[i]))
			str += tabpack(padr(::edbuffer[i], n), ::tabSize)+ ;
				ltrim(::edbuffer[i])+"&\n"
		next
		*/
	else
		aeval(::edbuffer, {|item| str:=str+item+"&\n"})
		/*
		for i=1 to ::lines
			str += ::edbuffer[i]+"&\n"
		next
		*/
	endif
  else
	hch := ::__hostcharset
	ch := ::charset
	if ::tabPack
		aeval(::edbuffer, {|item| ;
			str:=str+translate_charset(hch, ch,  ;
			tabpack(padr(item, len(item)-len(ltrim(item))), ts))+ ;
			    ltrim(item)+"&\n"})
	/*
	for i=1 to ::lines
		n := len(::edbuffer[i])-len(ltrim(::edbuffer[i]))
		str += translate_charset(::__hostcharset, ::charset, ;
			    tabpack(padr(::edbuffer[i], n), ::tabSize)+ ;
			    ltrim(::edbuffer[i]))+"&\n"
	next
	*/
	else
		aeval(::edbuffer, {|item| ;
			str:=str+translate_charset(hch, ch, item)+"&\n"	})
	/*
	for i=1 to ::lines
		str += translate_charset(::__hostcharset, ::charset, ::edbuffer[i])+"&\n"
	next
	*/
	endif
  endif
  ::updated:=.f.
RETURN str
