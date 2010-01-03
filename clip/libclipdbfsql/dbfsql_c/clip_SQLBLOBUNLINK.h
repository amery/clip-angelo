int
clip_SQLBLOBUNLINK(ClipMachine * ClipMachineMemory)
{
   int       conn_item = _clip_parni(ClipMachineMemory, 1);

   SQLCONN  *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);

   unsigned int OID = (unsigned int) _clip_parni(ClipMachineMemory, 2);

   if (!conn)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
       return 1;
    }
   if (conn->vtbl->blob_unlink)
      return conn->vtbl->blob_unlink(ClipMachineMemory, conn, OID);
   return 0;
}
