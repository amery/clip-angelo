static function br_addObject(str, color, type, fnc, size, name, line, pos)
local numb
	line := iif(line==NIL, ::doc:n_Line, line)
	pos := iif(pos==NIL, ::doc:n_Pos, pos)
	if valtype(str)=="C"
		str := translate_charset(::charset, host_charset(), str)
	endif
	if ::isRefer .and. type==NIL
		::doc:Button(str, ::hRefer, line, pos, , , ::clr, ::nRefer, ::iRefer)
		return .t.
	endif
	if type==NIL
		::doc:Text(str,line,pos,,,::clr)
		return .t.
	endif
	color := iif(color==NIL, ::clr, color)
	do case
	case type == "Get"
		::doc:Get(line,pos,str,size,color,,,,name)
	case type == "Button"
		color := ::__colors[4]+"/"+::__colors[5]
		::doc:Button(str, fnc, line, pos+2,,,color,name)
	case type == "Check"
		::doc:Check(line,pos,str,,color,,, name)
	case type == "Radio"
		::doc:Radio(line,pos,str, fnc,"",color,,, name)
	case type == "Textarea"
	case type == "Password"
		::doc:Get(line,pos,str,size,color,fnc,,,name)
	case type == "Browse"
		numb := ::doc:Get(line,pos,str,size,color,,,,name)
		color := ::__colors[4]+"/"+::__colors[5]
		::doc:Button("Browse", {|s, self| self:Browse_File(numb)}, line, ::doc:n_Pos+2,,,color,name)
	endcase
return
