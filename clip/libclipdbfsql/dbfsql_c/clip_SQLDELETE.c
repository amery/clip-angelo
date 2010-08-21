int
clip_SQLDELETE(ClipMachine * ClipMachineMemory)
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
   if (rowset->lastrec > 0)
   {
      int oldrecno = rowset->recno;

      int newrecno = 0, k;

      rowset->bof = rowset->eof = 0;
      if (rowset->curord)
      {
	 int eof;

	 if (sql_searchtree(ClipMachineMemory, rowset, rowset->curord))
	    return 1;
	 if ((eof = bt_next(rowset->curord->bt)))
	 {
	    bt_prev(rowset->curord->bt);
	 }
	 newrecno = *(int *) bt_key(rowset->curord->bt);
	 if (newrecno > rowset->recno)
	    newrecno--;
	 if (!eof)
	    bt_prev(rowset->curord->bt);
      }
      for (k = 0; k < rowset->ntags; k++)
      {
	 SQLORDER *order = (SQLORDER *) HashTable_fetch(rowset->orders,
							rowset->taghashes[k]);

	 if (sql_searchtree(ClipMachineMemory, rowset, order))
	    return 1;
	 if (sql_orderdel(ClipMachineMemory, rowset, rowset->taghashes[k]))
	    return 1;
      }
      rowset->conn->vtbl->delete_(rowset);
      rowset->lastrec--;
      rowset->loaded--;
      if (rowset->lastrec == 0)
      {
	 rowset->bof = rowset->eof = 1;
	 rowset->recno = 0;
      }
      else if (newrecno)
      {
	 rowset->recno = newrecno;
      }
      else if (rowset->recno > rowset->lastrec)
      {
	 rowset->recno = rowset->lastrec;
	 rowset->eof = 1;
      }
      for (k = 0; k < rowset->ntags; k++)
      {
	 SQLORDER *order = (SQLORDER *) HashTable_fetch(rowset->orders,
							rowset->taghashes[k]);

	 bt_first(order->bt);
	 do
	 {
	    if (bt_key(order->bt) && *(int *) bt_key(order->bt) > oldrecno)
	       (*(int *) bt_key(order->bt))--;
	 }
	 while (!bt_next(order->bt));
      }
   }
   return 0;
}
