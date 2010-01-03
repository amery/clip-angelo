int
clip_SQLDESTROYCONN(ClipMachine * ClipMachineMemory)
{
   int       conn_item = _clip_parni(ClipMachineMemory, 1);

   SQLCONN  *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);

   if (!conn)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
       return 1;
    }
   conn->vtbl->destroyconn(conn);
   _clip_destroy_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);
   return 0;
}
