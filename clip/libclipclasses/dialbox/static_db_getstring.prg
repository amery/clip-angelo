static function db_getString(self, elem, line)
local str, i, ielem
	if ("OBJIT"$elem)
		ielem := elem:objit
	endif

	do case
	case elem:type == _GETB_
		str := ielem:buffer
	case elem:type == _CHECK_
		str := "["+substr(ielem:Control:style, iif(ielem:Control:buffer, 2, 3),1)+"]"
	case elem:type == _RADIO_
		str := "("+iif(ielem:Control:value==line-ielem:row+1, "*", " ")+") "+ielem:Control:__items[line-ielem:row+1]:caption
	case elem:type == _GETTEXT_
		str := ""
		for i=1 to len(ielem:edit:edbuffer)
			str += ielem:edit:edbuffer[i]+"&\t"
		next
	otherwise
		str := elem:txt
	endcase
return str
