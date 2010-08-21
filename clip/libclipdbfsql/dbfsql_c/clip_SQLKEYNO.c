int
clip_SQLKEYNO(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   int keyno = 0;

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }
   if (rowset->curord)
   {
      if (!rowset->curord->bt)
	 keyno = rowset->recno;
      else if (!bt_first(rowset->curord->bt))
      {
	 keyno++;
	 while (rowset->recno != *(int *) bt_key(rowset->curord->bt))
	 {
	    bt_next(rowset->curord->bt);
	    keyno++;
	 }
      }
   }
   else
   {
      keyno = rowset->recno;
   }
   _clip_retni(ClipMachineMemory, keyno);
   return 0;
}
