int
clip_SQLSETHOT(ClipMachine * ClipMachineMemory)
{
   int rowset_item = _clip_parni(ClipMachineMemory, 1);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   rowset->hot = _clip_parl(ClipMachineMemory, 2);
   return 0;
}
