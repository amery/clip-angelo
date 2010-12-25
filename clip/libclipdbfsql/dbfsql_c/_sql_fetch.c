int
_sql_fetch(ClipMachine * ClipMachineMemory, SQLROWSET * rowset)
{
   if (rowset->recno > rowset->loaded)
   {
      if (rowset->conn->vtbl->fetch(ClipMachineMemory, rowset, rowset->recno - rowset->loaded, NULL, 0, NULL))
	 return 1;
   }
   if (rowset->done && (rowset->loaded == 0))
   {
      rowset->recno = 0;
      rowset->bof = rowset->eof = 1;
   }
   return 0;
}
