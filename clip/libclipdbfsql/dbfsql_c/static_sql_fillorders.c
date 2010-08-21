static int
sql_fillorders(ClipMachine * ClipMachineMemory, SQLROWSET * rowset)
{
   int k;

   int oldrecno = rowset->recno;

   if (rowset->conn->vtbl->fetch(ClipMachineMemory, rowset, 0, NULL, 0, NULL))
      return 1;

   for (rowset->recno = 1; rowset->recno <= rowset->lastrec; rowset->recno++)
   {
      for (k = 0; k < rowset->ntags; k++)
      {
	 if (sql_orderadd(ClipMachineMemory, rowset, rowset->taghashes[k]))
	    return 1;
      }
   }
   rowset->recno = oldrecno;
   return 0;
}
