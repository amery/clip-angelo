int
clip_SQLCREATEORDER(ClipMachine * ClipMachineMemory)
{
   ClipVar  *rmap = _clip_spar(ClipMachineMemory, 1);

   int       rowset_item = _clip_parni(ClipMachineMemory, 2);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   char     *tagname = _clip_parc(ClipMachineMemory, 3);

   ClipVar  *expr = _clip_par(ClipMachineMemory, 4);

   int       t4 = _clip_parinfo(ClipMachineMemory, 4);

   int       len = _clip_parni(ClipMachineMemory, 5);

   long      taghash = _clip_casehashstr(tagname);

   int       oldrecno;

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }
	 if (t4 != CHARACTER_type_of_ClipVarType && t4 != NUMERIC_type_of_ClipVarType
		 && t4 != CCODE_type_of_ClipVarType && t4 != PCODE_type_of_ClipVarType)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }

   if (rowset->conn->vtbl->fetch(ClipMachineMemory, rowset, 0, NULL, 0, NULL))
      return 1;

   if (sql_createorder(ClipMachineMemory, rmap, rowset, tagname, expr, len))
      return 1;
   rowset->ntags++;
   rowset->taghashes = realloc(rowset->taghashes, rowset->ntags * sizeof(long));
   rowset->taghashes[rowset->ntags - 1] = _clip_casehashstr(tagname);

   oldrecno = rowset->recno;
   for (rowset->recno = 1; rowset->recno <= rowset->lastrec; rowset->recno++)
    {
       if (sql_orderadd(ClipMachineMemory, rowset, taghash))
	  return 1;
    }
   rowset->recno = oldrecno;
   return 0;
}
