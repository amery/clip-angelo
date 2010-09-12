function __dbCopyXStruct(fname)
   local i,struct,fields:=dbstruct(),sel:=select(), __alias:=tmpalias()
   struct:={{"field_name","C",10,0},{"field_type","C",1,0},;
		{"field_len" ,"N",3 ,0},{"field_dec","N",4,0},;
		{"field_bin" ,"L",1 ,0},{"field_nil","L",1,0};
		}
   dbcreate(fname,struct)
   use (fname) new exclusive ALIAS (__alias)
   outlog(3,"copyxstruct",fname,fields)
   for i=1 to len(fields)
	   append blank
	   field->field_name:=fields[i][1]
	   field->field_type:=fields[i][2]
	   field->field_len :=fields[i][3]
	   field->field_dec :=fields[i][4]
	   if len(fields[i]) > 5 .and. valtype(fields[i])=="L"
			field->field_bin :=fields[i][5]
	   endif
	   if len(fields[i]) > 6 .and. valtype(fields[i])=="L"
			field->field_nil :=fields[i][6]
	   endif
   next
   commit
   use
   select(sel)
return
