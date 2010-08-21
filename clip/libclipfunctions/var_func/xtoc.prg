***************************************************************
function xtoc( var,len,dec )
   local ret:="",sw:=valtype(var)
   do case
	  case sw=="A"
	   ret=LANG_ARRAY_STRING
	  case sw=="B"
	   ret=LANG_CBLOCK_STRING
	  case sw=="C"
	   ret=var
	  case sw=="D"
		if empty(var)
			ret=space(10)
		else
			ret=dtos(var)
		endif
	  case sw=="L"
	   ret=iif(var,"T","F") //iif(var,LANG_YES_CHAR,LANG_NO_CHAR)
	  case sw=="N"
	   ret=str(var,len,dec)
	  case sw=="M"
	   ret=var
	  case sw=="O"
	   ret=LANG_OBJECT_STRING
	  case sw=="U"
	   ret=""
	  otherwise
	   ret=LANG_UNKNOWN_STRING
   end
return ret
