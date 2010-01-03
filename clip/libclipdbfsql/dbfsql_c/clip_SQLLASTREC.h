int
clip_SQLLASTREC(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }
   _clip_retni(ClipMachineMemory, rowset->lastrec);
   return 0;
}
