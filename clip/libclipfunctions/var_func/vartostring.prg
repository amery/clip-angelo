***************************************************************
function varToString( var,len,dec, lAtype )
   local ret:="",sw:=valtype(var), i, arr
   lAtype:=iif(lAtype==NIL,.t.,lAtype)
   do case
	  case sw=="A"
		if lAtype
			ret="aclone({"
		else
			ret="{"
		endif
		for i:=1 to len(var)
			ret += varToString(var[i], len, dec, lAtype) + ","
		next
		if len(var) > 0
			ret := substr(ret, 1, len(ret)-1)
		endif
		if lAtype
			ret += "})"
		else
			ret += "}"
		endif
	  case sw=="C"
		ret='"'+var+'"'
	  case sw=="D"
		ret='"'+dtoc(var)+'"'
	  case sw=="T"
		ret='"'+ttoc(var)+'"'
	  case sw=="L"
		ret=iif(var,".T.",".F.")
	  case sw=="N"
		ret=str(var,len,dec)
	  case sw=="M"
		ret='"'+var+'"'
	  case sw=="O"
		if lAtype
			ret := "obj:=map()"
			arr := mapkeys(var)
			for i:= 1 to len(arr)
				ret += ", obj:"+alltrim(hashname(arr[i]))+":="+varToString(var[arr[i]], len, dec, lAtype)
			next
		else
			ret := "{object}"
		endif
	  case sw=="U"
		ret="NIL"
	  otherwise
		ret="NIL"
   end
return ret
