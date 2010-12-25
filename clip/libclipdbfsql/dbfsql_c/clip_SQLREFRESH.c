int
clip_SQLREFRESH(ClipMachine * ClipMachineMemory)
{
   int rowset_item = _clip_parni(ClipMachineMemory, 1);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   SQLSTMT *stmt = (SQLSTMT *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 2),
						  _C_ITEM_TYPE_SQL);

   ClipVar *ap = _clip_par(ClipMachineMemory, 3);

   int k;

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }
   if (!stmt)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOSTATEMENT, er_nostatement);
      return 1;
   }
   if (rowset->conn->vtbl->fetch(ClipMachineMemory, rowset, 0, NULL, 0, NULL))
      return 1;
   for (k = 0; k < rowset->ntags; k++)
   {
      if (sql_orderdel(ClipMachineMemory, rowset, rowset->taghashes[k]))
	 return 1;
   }
   if (stmt->conn->vtbl->refresh(ClipMachineMemory, rowset, stmt, ap) == -1)
      return 1;
   for (k = 0; k < rowset->ntags; k++)
   {
      if (sql_orderadd(ClipMachineMemory, rowset, rowset->taghashes[k]))
	 return 1;
   }

   return 0;
}
