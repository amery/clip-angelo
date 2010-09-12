int
clip_SQLCREATEROWSET(ClipMachine * ClipMachineMemory)
{
   ClipVar *rmap = _clip_spar(ClipMachineMemory, 1);

   SQLSTMT *stmt = (SQLSTMT *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 2),
						  _C_ITEM_TYPE_SQL);

   ClipVar *ap = _clip_par(ClipMachineMemory, 3);

   ClipVar *idname = _clip_par(ClipMachineMemory, 4);

   ClipVar *orders = _clip_par(ClipMachineMemory, 5);

   const char *gen_idSQL = _clip_parc(ClipMachineMemory, 6);

   SQLROWSET *rowset;

   ClipVar *oname, *oexpr, *olen, *el;

   long dims[1];

   int i;

   if (!stmt)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOSTATEMENT, er_nostatement);
      return 1;
   }
   if (orders && orders->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType && orders->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, EG_ARG, er_badorders);
      return 1;
   }

   rowset = calloc(1, stmt->conn->vtbl->sizeof_rowset);
   rowset->orders = new_HashTable();
   rowset->conn = stmt->conn;
   rowset->stmt = stmt;
   if (orders && orders->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      rowset->ntags = orders->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      rowset->taghashes = calloc(orders->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar, sizeof(long));
      for (i = 0; i < orders->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
      {
	 dims[0] = i;
	 if (!(el = _clip_aref(ClipMachineMemory, orders, 1, dims)))
	 {
	    _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, EG_ARG, er_badorders);
	    return 1;
	 }
	 dims[0] = 0;
	 if (!(oname = _clip_aref(ClipMachineMemory, el, 1, dims)) || oname->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	 {
	    _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, EG_ARG, er_badorders);
	    return 1;
	 }
	 rowset->taghashes[i] = _clip_casehashstr(oname->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 dims[0] = 1;
	 if (!(oexpr = _clip_aref(ClipMachineMemory, el, 1, dims)))
	 {
	    _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, EG_ARG, er_badorders);
	    return 1;
	 }
	 dims[0] = 2;
	 if ((olen = _clip_aref(ClipMachineMemory, el, 1, dims)) && olen->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	 {
	    _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, EG_ARG, er_badorders);
	    return 1;
	 }
	 if (sql_createorder
	     (ClipMachineMemory, rmap, rowset, oname->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, oexpr, (olen ? olen->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar : 0)))
	    return 1;
      }
   }
   if (stmt->conn->vtbl->createrowset(ClipMachineMemory, rowset, ap, idname, gen_idSQL))
   {
      _clip_destroy_c_item(ClipMachineMemory, rowset->rowset_item, _C_ITEM_TYPE_SQL);
      return 1;
   }

   if (!rowset->unknownrows && rowset->lastrec == 0)
   {
      rowset->bof = rowset->eof = 1;
      rowset->recno = 0;
   }
   else
   {
      rowset->recno = 1;
      if (_sql_fetch(ClipMachineMemory, rowset))
	 return 1;
   }

   _clip_retni(ClipMachineMemory, rowset->rowset_item);
   return 0;
}
