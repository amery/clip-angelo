static function parse_Input(tag, insTag)
local item:=map(), ind, clr, clrh, s_file, numb, str
	ind := len(::form)
	item:type := "TEXT"
	item:value := replicate(" ", 10)
	item:name := ""
	item:size := 0
	item:maxlength := 0
	item:checked := .f.
	insTag := iif(insTag==NIL, .t., insTag)
	if "TYPE"$tag:fields
		item:type := upper(tag:fields:TYPE)
	endif
	if "VALUE"$tag:fields
		item:value := tag:fields:VALUE
	endif
	if "NAME"$tag:fields
		item:name := upper(tag:fields:NAME)
	endif
	if "SIZE"$tag:fields
		item:size := &(tag:fields:SIZE)
	endif
	if "MAXLENGTH"$tag:fields
		item:maxlength := &(tag:fields:MAXLENGTH)
	endif
	if "CHECKED"$tag:fields
		item:checked := .t.
	endif
	if item:size == 0
		item:size := len(item:value)
	endif
	if item:maxlength == 0
		item:maxlength := max(len(item:value), int(::width/3))
	endif
	item:value := padr(item:value, item:maxlength)
	clr:=::__colors[7]+","+::__colors[8]
	if !insTag
		return item:maxlength
	endif
	do case
	case item:type == "TEXT"
		::addObject(item:value, clr, "Get", , item:size, item:name)
	case item:type == "CHECKBOX"
		::addObject(item:checked, clr, "Check",,,item:name)
	case item:type == "FILE"
		s_file := " ":replicate(256)
		::addObject(s_file, clr, "Browse", "", int(::width/3), item:name)
	case item:type == "HIDDEN"
	case item:type == "IMAGE"
		::parse_Image()
	case item:type == "PASSWORD"
		::addObject(item:value, clr, "Password", "@Q", item:size, item:name)
	case item:type == "RADIO"
		::addObject(item:checked, clr, "Radio", item:name,, item:name)
	case item:type == "RESET"
	case item:type == "SUBMIT"
		item:name := iif(empty(item:name), "SUBMIT", item:name)
		str := ::form[::ndxForm]:action
		::addObject(item:name,,"Button", {|s, self| self:newRefer(str)},,item:name )

	endcase
return item:maxlength
