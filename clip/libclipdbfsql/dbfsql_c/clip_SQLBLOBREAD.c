int
clip_SQLBLOBREAD(ClipMachine * ClipMachineMemory)
{
   int conn_item = _clip_parni(ClipMachineMemory, 1);

   SQLCONN *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);

   int oid_fd = _clip_parni(ClipMachineMemory, 2);

   char *buffer = _clip_parc(ClipMachineMemory, 3);

   int length = _clip_parni(ClipMachineMemory, 4);

   if (!conn)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
      return 1;
   }
   if (conn->vtbl->blob_read)
      return conn->vtbl->blob_read(ClipMachineMemory, conn, oid_fd, buffer, length);
   return 0;
}
