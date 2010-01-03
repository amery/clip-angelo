int
clip_SQLNEWID(ClipMachine * ClipMachineMemory)
{
   SQLCONN  *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
						   _C_ITEM_TYPE_SQL);

   SQLSTMT  *stmt = (SQLSTMT *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 2),
						   _C_ITEM_TYPE_SQL);

   if (!conn)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
       return 1;
    }
   if (!stmt)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOSTATEMENT, er_nostatement);
       return 1;
    }
   conn->vtbl->newid(ClipMachineMemory, stmt);
   return 0;
}
