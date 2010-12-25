int
clip_SQLFREESTMT(ClipMachine * ClipMachineMemory)
{
   int stmt_item = _clip_parni(ClipMachineMemory, 1);

   SQLSTMT *stmt = (SQLSTMT *) _clip_fetch_c_item(ClipMachineMemory, stmt_item, _C_ITEM_TYPE_SQL);

   if (!stmt)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOSTATEMENT, er_nostatement);
      return 1;
   }

   _clip_destroy_c_item(ClipMachineMemory, stmt->stmt_item, _C_ITEM_TYPE_SQL);
   return 0;
}
