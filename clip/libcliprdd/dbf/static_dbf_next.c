static int
dbf_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int filtok, const char *__PROC__)
{
   int lastrec, res, er, deleted;

   if (rd->eof)
      return 0;
   rd->bof = rd->v_bof = rd->eof = 0;

   if ((er = dbf_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;
   if (rd->filter && rd->filter->list)
   {
      while (++rd->filter->cursor < rd->filter->listlen)
      {
	 if ((er = dbf_rawgoto(ClipMachineMemory, rd, rd->filter->list[rd->filter->cursor], 0, __PROC__)))
	    return er;
	 if (!(ClipMachineMemory->flags & DELETED_FLAG))
	    return 0;
	 if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	    return er;
	 if (!deleted)
	    return 0;
      }
      rd->eof = 1;
      if ((er = dbf_rawgoto(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
      if (rd->filter->listlen == 0)
      {
	 rd->bof = rd->v_bof = 1;
	 rd->filter->cursor = 0;
      }
      return 0;
   }
   while (1)
   {
      rd->recno++;
      if (rd->filter && rd->filter->rmap)
	 while (rd->recno <= lastrec && !_rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno))
	    rd->recno++;
      if ((er = dbf_rawgoto(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	 return er;
      if (rd->recno > lastrec)
      {
	 rd->eof = 1;
	 return 0;
      }
      if (filtok)
	 break;
      if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
	 return er;
      if (res)
	 break;
   }
   return 0;
}
