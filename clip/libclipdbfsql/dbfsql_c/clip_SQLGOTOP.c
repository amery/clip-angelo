int
clip_SQLGOTOP(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }
   if (rowset->done && !rowset->lastrec)
   {
      rowset->bof = rowset->eof = 1;
      rowset->recno = 0;
      return 0;
   }
   if (rowset->curord)
   {
      if (!rowset->curord->bt || bt_first(rowset->curord->bt))
      {
	 rowset->bof = rowset->eof = 1;
	 rowset->recno = 0;
      }
      else
      {
	 rowset->recno = *(int *) bt_key(rowset->curord->bt);
      }
   }
   rowset->bof = rowset->eof = 0;
   rowset->recno = 1;
   if (_sql_fetch(ClipMachineMemory, rowset))
      return 1;
   return 0;
}
