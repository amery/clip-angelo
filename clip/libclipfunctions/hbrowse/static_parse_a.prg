static function parse_A(tag)
local name, target, title, href, txt
	::isRefer := .t.
	::hRefer := {|| alert("Ups!!")}
	::nRefer := ""
	if "NAME"$tag:fields
		::nRefer := tag:fields:NAME
	endif
	if "TARGET"$tag:fields
		target := tag:fields:TARGET
	endif
	if "TITLE"$tag:fields
		title := tag:fields:TITLE
	endif
	if "HREF"$tag:fields
		href := tag:fields:HREF
		::iRefer := href
		::hRefer := {|s, self| self:newRefer(href)}
	endif

	if ::center
		::doc:n_Pos := (::width-len(txt))/2+::doc:marginLeft
	endif

	::new_clr(::__colors[2]+","+::__colors[3])
return
