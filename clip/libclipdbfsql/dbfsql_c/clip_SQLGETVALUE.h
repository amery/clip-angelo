int
clip_SQLGETVALUE(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   int       fieldno = _clip_parni(ClipMachineMemory, 2) - 1;

   char     *data;

   int       len;

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
   if (rowset->recno)
    {
       data = rowset->conn->vtbl->getvalue(rowset, fieldno, &len);
       if (data)
	{
	   _clip_retcn(ClipMachineMemory, data, len);
	}
    }
   return 0;
}
