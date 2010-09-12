static function sstr
parameters per,nect,dec
local n:=""
 do case
    case stype("per")=="C"
	n=per
    case stype("per")=="D"
	n=dtoc(per)
    case stype("per")=="N"
	 if valtype(nect)=="U" .or. valtype(dec)=="U"
	   n=str(per)
	 else
	   n=str(per,nect,dec)
	 endif
 endcase
return n
