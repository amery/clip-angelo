static int
sql_orderdel(ClipMachine * ClipMachineMemory, SQLROWSET * rowset, long taghash)
{
   SQLORDER *order = (SQLORDER *) HashTable_fetch(rowset->orders, taghash);

   if (!order)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOORDER, er_noorder);
       return 1;
    }
   return bt_del(order->bt);
}
