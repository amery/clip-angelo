int
clip_SQLSTART(ClipMachine * ClipMachineMemory)
{
   int conn_item = _clip_parni(ClipMachineMemory, 1);

   SQLCONN *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);

   const char *p1 = _clip_parc(ClipMachineMemory, 2);

   const char *p2 = _clip_parc(ClipMachineMemory, 3);

   if (!conn)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
      return 1;
   }
   if (conn->vtbl->start)
      return conn->vtbl->start(ClipMachineMemory, conn, p1, p2);
   return 0;
}
