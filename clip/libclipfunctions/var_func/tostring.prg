***************************************************************
function toString( var,len,dec )
   local ret:="",sw:=valtype(var)
   do case
	  case sw=="A"
	   ret=LANG_ARRAY_STRING
	  case sw=="B"
	   ret=LANG_CBLOCK_STRING
	  case sw=="C"
	   ret=var
	  case sw=="D"
	   ret=dtoc(var)
	  case sw=="T"
	   ret=ttoc(var)
	  case sw=="L"
	   ret=iif(var,LANG_YES_STRING,LANG_NO_STRING)
	  case sw=="N"
	   ret=str(var,len,dec)
	  case sw=="M"
	   ret=LANG_MEMO_STRING
	  case sw=="O"
	   ret=LANG_OBJECT_STRING
	  case sw=="U"
	   ret=LANG_UNDEF_STRING
	  otherwise
	   ret=LANG_UNKNOWN_STRING
   end
return ret
