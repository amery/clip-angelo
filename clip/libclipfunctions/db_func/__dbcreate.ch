function __dbCreate(fname1, fname2, rdd, new, al)
   local i,struct:={}, __alias:=tmpalias(), sel:=select()
   local ao := set(_SET_AUTOPEN,.f.)
   new=iif(new==NIL,.f.,new)
   if empty(fname2)
	struct:={{"field_name","C",10,0},{"field_type","C",1,0},;
		{"field_len" ,"N",3 ,0},{"field_dec","N",4,0}}
   else
	use (fname2) new shared alias (__alias)
	if neterr()
		select getarea(fname2)
		go top
	endif
	do while !eof()
	   aadd(struct,{alltrim(field->field_name),field->field_type,field->field_len,field->field_dec})
	   skip
	enddo
	use
   endif
   select(sel)
   outlog(3,"create <file>",fname1,struct)
   dbcreate(fname1,struct,rdd)
   if new
	  use (fname1) alias (al) via (rdd) exclusive NEW
   else
	  use (fname1) alias (al) via (rdd) exclusive
   endif
   set(_SET_AUTOPEN,ao)
return
