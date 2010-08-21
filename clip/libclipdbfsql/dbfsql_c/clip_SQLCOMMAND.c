int
clip_SQLCOMMAND(ClipMachine * ClipMachineMemory)
{
   SQLSTMT *stmt = (SQLSTMT *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
						  _C_ITEM_TYPE_SQL);

   ClipVar *ap = _clip_par(ClipMachineMemory, 2);

   int res;

   if (!stmt)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOSTATEMENT, er_nostatement);
      return 1;
   }
   if ((res = stmt->conn->vtbl->command(ClipMachineMemory, stmt, ap)) == -1)
   {
      _clip_destroy_c_item(ClipMachineMemory, stmt->stmt_item, _C_ITEM_TYPE_SQL);
      return 1;
   }
   _clip_retni(ClipMachineMemory, res);
   return 0;
}
