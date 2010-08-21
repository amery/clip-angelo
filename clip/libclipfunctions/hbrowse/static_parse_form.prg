static function parse_Form(tag)
local item:=map()
	item:action := ::url:makeUrl()
	item:enctype := ""
	item:method := "GET"
	item:target := ""
	item:index := len(::form)+1
	item:items := {}
	if "ACTION"$tag:fields
		item:action := tag:fields:ACTION
	endif
	if "ENCTYPE"$tag:fields
		item:enctype := tag:fields:ENCTYPE
	endif
	if "METHOD"$tag:fields
		item:method := tag:fields:METHOD
	endif
	if "TARGET"$tag:fields
		item:target := tag:fields:TARGET
	endif
	aadd(::form, item)
	::ndxForm := len(::form)
return .t.
