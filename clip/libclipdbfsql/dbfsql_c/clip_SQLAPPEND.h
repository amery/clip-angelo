int
clip_SQLAPPEND(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }
   if (rowset->conn->vtbl->fetch(ClipMachineMemory, rowset, 0, NULL, 0, NULL))
      return 1;
   rowset->conn->vtbl->append(rowset);
   rowset->recno = rowset->lastrec;
   rowset->bof = rowset->eof = 0;
   rowset->newrec = 1;
   return 0;
}
