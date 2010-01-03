int
clip_SQLSETVALUE(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   int       fieldno = _clip_parni(ClipMachineMemory, 2) - 1;

   int       len, k;

   char     *value = _clip_parcl(ClipMachineMemory, 3, &len);

   SQLORDER *order;

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }
   if (fieldno < 0 || fieldno >= rowset->nfields)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOFIELD, er_nofield);
       return 1;
    }

   if (!value)
      len = 0;
   if (!rowset->newrec)
    {
       for (k = 0; k < rowset->ntags; k++)
	{
	   order = (SQLORDER *) HashTable_fetch(rowset->orders, rowset->taghashes[k]);
	   if (sql_searchtree(ClipMachineMemory, rowset, order))
	      return 1;
	   if (sql_orderdel(ClipMachineMemory, rowset, rowset->taghashes[k]))
	      return 1;
	}
    }
   rowset->conn->vtbl->setvalue(rowset, fieldno, value, len);
   if (!rowset->newrec)
    {
       for (k = 0; k < rowset->ntags; k++)
	{
	   order = (SQLORDER *) HashTable_fetch(rowset->orders, rowset->taghashes[k]);
	   if (sql_orderadd(ClipMachineMemory, rowset, rowset->taghashes[k]))
	      return 1;
	}
    }
   return 0;
}
