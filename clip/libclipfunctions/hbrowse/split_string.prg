function split_string(str, width, prewidth)
local arr:={}, i, pr, s, es, rs
	prewidth := iif(prewidth==NIL, width, prewidth)
	str := replace_symbols(str)
	if len(str)<=prewidth
		aadd(arr, str)
		return arr
	endif
	pr := str:split(" ")
	s := ""
	rs := " ":replicate(len(str)-len(rtrim(str)))
	es := " ":replicate(len(str)-len(ltrim(str)))
	for i=1 to len(pr)
		if empty(pr[i])
			loop
		endif
		if len(s+es+pr[i])>prewidth
			arr:aadd(s)
			s := pr[i]
			prewidth := width
		else
			s += es+pr[i]
		endif
		es := " "
	next
	arr:aadd(s+rs)
return arr
