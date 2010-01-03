int
clip_SQLSEEK(ClipMachine * ClipMachineMemory)
{
   int       rowset_item = _clip_parni(ClipMachineMemory, 1);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   ClipVar  *val = _clip_par(ClipMachineMemory, 2);

   int       soft = _clip_parl(ClipMachineMemory, 3);

   int       found;

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }

   if (sql_seek(ClipMachineMemory, rowset, val, soft, &found))
      return 1;
   _clip_retl(ClipMachineMemory, found);
   return 0;
}
