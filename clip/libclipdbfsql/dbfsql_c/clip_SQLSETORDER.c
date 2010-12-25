int
clip_SQLSETORDER(ClipMachine * ClipMachineMemory)
{
   int rowset_item = _clip_parni(ClipMachineMemory, 1);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   const char *tagname = _clip_parc(ClipMachineMemory, 2);

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }
   if (rowset->curord)
      _clip_retc(ClipMachineMemory, rowset->curord->name);
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
   {
      if (!tagname || !tagname[0])
      {
	 rowset->curord = NULL;
      }
      else
      {
	 rowset->curord = HashTable_fetch(rowset->orders, _clip_casehashstr(tagname));
      }
   }
   return 0;
}
