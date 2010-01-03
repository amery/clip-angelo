int
clip_SQLADDKEYS(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   int       k;

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }
   for (k = 0; k < rowset->ntags; k++)
    {
       if (sql_orderadd(ClipMachineMemory, rowset, rowset->taghashes[k]))
	  return 1;
    }
   rowset->newrec = 0;
   return 0;
}
