static function db_getStrElem(self, elem, line)
local str, ielem
	if "OBJIT"$elem
		ielem := elem:OBJIT
	endif

	do case
	case elem:type == _GETB_
		str := substr(ielem:buffer, 1, ielem:__winLen)
	case elem:type == _CHECK_
		str := "["+substr(ielem:Control:style, iif(ielem:Control:buffer, 2, 3),1)+"]"
	case elem:type == _RADIO_
		str := "("+iif(ielem:Control:value==line-ielem:row+1, "*", " ")+") "+ielem:Control:__items[line-ielem:row+1]:caption
	case elem:type == _GETTEXT_
		str := padr(ielem:edit:edbuffer[min(line-ielem:row+1+ielem:edit:line-ielem:edit:rowWin, ielem:edit:lines)], elem:nr-elem:nl+1)
	otherwise
		str := elem:txt
	endcase
return str
