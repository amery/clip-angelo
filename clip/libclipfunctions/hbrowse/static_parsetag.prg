static function parseTag(tag, insTag)
local typ:=upper(tag:tagname), ret:=0
	insTag := iif(insTag==NIL, .t., insTag)
	do case
	case typ == "HTML" .or. typ == "BODY"
		::startHtml := .t.
	case typ == "/HTML" .or. typ == "/BODY"
		::startHtml := .f.
	case typ == "TITLE"
		::startHtml := .t.
		ret := ::parse_Title()
	case typ == "HEAD"
		::newLine()
		::parse_Head(tag)
	case typ == "/HEAD"
		::newLine()
		::rest_clr()
	case search("/[Hh][12345]", typ)
		::rest_clr()
		::newLine()
	case search("[Hh][12345]", typ)
		::newLine()
		::new_clr(::__colors[6])
		::parse_H(&(substr(typ, 2)))
	case typ == "CENTER"
		::center := .t.
	case typ == "/CENTER"
		::center := .f.
	case typ == "FONT"
		::parse_Font(tag)
	case typ == "/FONT"
		::rest_clr()
	case typ == "A"
		::parse_A(tag)
	case typ == "/A"
		/*
		if !empty(::hRefer) .or. !empty(::nRefer)
			::addObject("")
		endif
		*/
		::isRefer := .f.
		::hRefer := ""
		::nRefer := ""
		::iRefer := ""
		::rest_clr()
	case typ == "PRE"
		::newLine()
		::pre := .t.
	case typ == "/PRE"
		::pre := .f.
		::newLine()
	case typ == "HR"
		::newLine()
		ret := ::parse_Hr(tag)
		::newLine()
	case typ == "BR"
		::newLine()
	case typ == "P"
		::newLine(2)
	case typ == "I"
		::new_clr(::__colors[9])
	case typ == "/I"
		::rest_clr()
	case typ == "CODE"
		::new_clr(::__colors[10])
	case typ == "/CODE"
		::rest_clr()
	case typ == "VAR"
		::new_clr(::__colors[9])
	case typ == "/VAR"
		::rest_clr()
	case typ == "UL"
		::newLine(2)
		::parse_Ul()
	case typ == "OL"
		::newLine(2)
		::parse_Ol(tag)
	case typ == "/UL" .or. typ == "/OL"
		::width += iif(len(::lists)>0, LISTDELTA, 0)
		::doc:marginLeft -= iif(len(::lists)>0, LISTDELTA, 0)
		::doc:marginLeft := max(::doc:marginLeft, MARGIN_LEFT)
		if len(::lists) != 0
			asize(::lists, len(::lists)-1)
		endif
		::rest_clr()
		 ::newLine()
	case typ == "LI"
		::newLine()
		ret := ::parse_Li(tag)
	case typ == "/LI"
		abs(1)
	case typ == "DT"
		if ::isDt > 0
			::isDt --
			::width += TAB_SIZE
			::doc:marginLeft -= TAB_SIZE
		endif
		::doc:marginLeft := max(::doc:marginLeft, MARGIN_LEFT)
		::newLine()
	case typ == "DD"
		if ::isDt == 0
			::isDt ++
			::width -= TAB_SIZE
			::doc:marginLeft += TAB_SIZE
		endif
		::newLine()
	case typ == "/DD"
		if ::isDt > 0
			::isDt --
			::width += TAB_SIZE
			::doc:marginLeft -= TAB_SIZE
		endif
		::doc:marginLeft := max(::doc:marginLeft, MARGIN_LEFT)
	case typ == "TABLE"
		::newLine()
		ret := ::parse_Table(tag, ::width)
	case typ == "FORM"
		::isForm := ::parse_Form(tag)
		::newLine()
	case typ == "/FORM"
		::isForm := .f.
	case typ == "INPUT"
		if ::isForm
			ret := ::parse_Input(tag, insTag)
		endif
	case typ == "IMG"
		ret := ::parse_Image(tag)
	case typ == "TEXTAREA"
		if ::isForm
			ret := ::parse_TextArea(tag, ::width)
		endif
	case typ == "META"
		::parse_Meta(tag)
	endcase
return ret
