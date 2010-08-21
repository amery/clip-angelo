function replace_symbols(str, space_charone)
local lsp
	lsp := len(str)-len(ltrim(str))
	space_charone := iif(space_charone==NIL, .t., space_charone)
	if space_charone
		str :=  " ":replicate(lsp)+charone(" ", ltrim(str))
	endif
	str := str:strtran("&#060;", "<")
	str := str:strtran("&lt;", "<")
	str := str:strtran("&gt;", ">")
	str := str:strtran("&#062;", ">")
	str := str:strtran("&nbsp;", " ")
	str := str:strtran("&quot;", '"')
	str := str:strtran("&#169;", "(C)")
	str := str:strtran("&#169", "(C)")
return str
