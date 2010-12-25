static function te_setCharset(newchs)
local i, hch, ch
	::charset := newchs
	hch := ::__hostcharset
	ch := ::charset
	aeval(::edbuffer, {|item| item := translate_charset(ch, hch, item, .t.)})
	//i:=0
	//aeval(::edbuffer, {|item| i:=i+1,item := str(i)+" :"+item}, 1, ::lines)
	//alert(str(i))
	::refresh()
return
