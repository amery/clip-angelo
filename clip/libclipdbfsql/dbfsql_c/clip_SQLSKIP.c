int
clip_SQLSKIP(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   int rows = _clip_parni(ClipMachineMemory, 2);

   int oldrecno, skipped = 0;

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }
   if (!rows || (rowset->bof && rowset->eof))
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   if (!((rows > 0 && rowset->eof) || (rows < 0 && rowset->bof)))
   {
      rowset->bof = rowset->eof = 0;
      if (rowset->curord)
      {
	 int i, j;

	 if (sql_searchtree(ClipMachineMemory, rowset, rowset->curord))
	    return 1;
	 for (i = 0, j = 0; i < abs(rows); i++, j++)
	 {
	    if (rows > 0)
	    {
	       if (bt_next(rowset->curord->bt))
	       {
		  rowset->eof = 1;
		  break;
	       }
	    }
	    else
	    {
	       if (bt_prev(rowset->curord->bt))
	       {
		  rowset->bof = 1;
		  break;
	       }
	    }
	 }
	 rowset->recno = *(int *) bt_key(rowset->curord->bt);
	 skipped = j;
      }
      else
      {
	 oldrecno = rowset->recno;
	 rowset->recno += rows;
	 if (_sql_fetch(ClipMachineMemory, rowset))
	    return 1;
	 if (rowset->recno < 1)
	 {
	    rowset->recno = 1;
	    rowset->bof = 1;
	 }
	 else if (rowset->done && rowset->recno > rowset->lastrec)
	 {
	    rowset->recno = rowset->lastrec;
	    rowset->eof = 1;
	 }
	 skipped = rowset->recno - oldrecno;
      }
   }
   _clip_retni(ClipMachineMemory, skipped);
   return 0;
}
