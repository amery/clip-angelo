int
clip_SQLGOTO(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   int       row = _clip_parni(ClipMachineMemory, 2);

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }
   if (rowset->lastrec > 0 || rowset->unknownrows)
    {
       rowset->bof = rowset->eof = 0;
       rowset->recno = row;
       if (_sql_fetch(ClipMachineMemory, rowset))
	  return 1;
       if (rowset->done && row > rowset->lastrec)
	{
	   rowset->recno = rowset->lastrec;
	   rowset->eof = 1;
	}
       else if (row < 1)
	{
	   rowset->recno = 1;
	   rowset->bof = 1;
	}
    }
   _clip_retni(ClipMachineMemory, rowset->recno);
   return 0;
}
