int
clip_SQLFIELDNULLABLE(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   int fieldno = _clip_parni(ClipMachineMemory, 2);

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }
   if (fieldno < 1 || fieldno > rowset->nfields)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOFIELD, er_nofield);
      return 1;
   }
   _clip_retl(ClipMachineMemory, !rowset->fields[fieldno - 1].notnull);
   return 0;
}
