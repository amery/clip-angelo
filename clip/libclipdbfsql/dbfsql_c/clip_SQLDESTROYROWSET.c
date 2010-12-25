int
clip_SQLDESTROYROWSET(ClipMachine * ClipMachineMemory)
{
   int rowset_item = _clip_parni(ClipMachineMemory, 1);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   int r;

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }

   r = HashTable_first(rowset->orders);
   while (r)
   {
      SQLORDER *order = HashTable_current(rowset->orders);

      _clip_destroy(ClipMachineMemory, order->block);
      free(order->block);
      if (order->bt)
	 bt_destroy(order->bt);
      free(order->rmap);
      free(order->name);
      free(order);
      r = HashTable_next(rowset->orders);
   }
   delete_HashTable(rowset->orders);

   if (rowset->ids)
      free(rowset->ids);

   if (!rowset->done)
      _clip_destroy_c_item(ClipMachineMemory, rowset->stmt->stmt_item, _C_ITEM_TYPE_SQL);

   _clip_destroy_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);
   return 0;
}
