int
clip_SQLPREPARE(ClipMachine * ClipMachineMemory)
{
   SQLCONN  *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
						   _C_ITEM_TYPE_SQL);

   char     *sql = _clip_parc(ClipMachineMemory, 2);

   int       res;

   if (!conn)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
       return 1;
    }
   if (!sql)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOSQL, er_nosql);
       return 1;
    }
   if ((res = conn->vtbl->prepare(ClipMachineMemory, conn, sql)) == -1)
      return 1;
   _clip_retni(ClipMachineMemory, res);
   return 0;
}
