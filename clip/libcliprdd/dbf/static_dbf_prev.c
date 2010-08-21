static int
dbf_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int res, deleted, lastrec, er;

   if (rd->bof)
      return 0;
   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
   {
      int oldcursor = rd->filter->cursor;

      while (rd->filter->cursor > 0)
      {
	 if ((er = dbf_rawgoto(ClipMachineMemory, rd, rd->filter->list[--rd->filter->cursor], 0, __PROC__)))
	    return er;
	 if (!(ClipMachineMemory->flags & DELETED_FLAG))
	    return 0;
	 if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	    return er;
	 if (!deleted)
	    return 0;
      }
      rd->bof = rd->v_bof = 1;
      if (rd->filter->listlen == 0)
      {
	 rd->eof = 1;
	 if ((er = dbf_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	    return er;
	 if ((er = dbf_rawgoto(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
      }
      else
      {
	 rd->filter->cursor = oldcursor;
	 if ((er = dbf_rawgoto(ClipMachineMemory, rd, rd->filter->list[oldcursor], 0, __PROC__)))
	    return er;
      }
      return 0;
   }

   while (1)
   {
      rd->recno--;
      if (rd->filter && rd->filter->rmap)
	 while (rd->recno > 0 && !_rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno))
	    rd->recno--;
      if (rd->recno <= 0)
      {
	 if ((er = dbf_gotop(ClipMachineMemory, rd, __PROC__)))
	    return er;
	 rd->bof = rd->v_bof = 1;
	 return 0;
      }
      if ((er = dbf_rawgoto(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	 return er;
      if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
	 return er;
      if (res)
	 break;
   }
   return 0;
}
