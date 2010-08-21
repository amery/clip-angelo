static int
dbf_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   unsigned int lastrec;

   int res, deleted, er;

   if ((er = dbf_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
      return er;
   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
   {
      if (rd->filter->listlen == 0)
      {
	 rd->bof = rd->v_bof = rd->eof = 1;
	 if ((er = dbf_rawgoto(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 rd->filter->cursor = 0;
      }
      else
      {
	 rd->filter->cursor = rd->filter->listlen;
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
	 rd->eof = rd->bof = rd->v_bof = 1;
	 if ((er = dbf_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	    return er;
	 if ((er = dbf_rawgoto(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
      }
      return 0;
   }

   if (lastrec == 0)
   {
      rd->bof = rd->v_bof = rd->eof = 1;
      if ((er = dbf_rawgoto(ClipMachineMemory, rd, 1, 0, __PROC__)))
	 return er;
      return 0;
   }

   rd->bof = rd->v_bof = rd->eof = 0;
   rd->recno = lastrec;
   if (rd->filter && rd->filter->rmap)
   {
      while (rd->recno >= 1 && !_rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno))
	 rd->recno--;
   }
   if (rd->recno < 1)
   {
      rd->eof = 1;
      if ((er = dbf_rawgoto(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
      return 0;
   }
   if ((er = dbf_rawgoto(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
      return er;

   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
      return er;
   if (!res)
   {
      if ((er = dbf_prev(ClipMachineMemory, rd, __PROC__)))
	 return er;
      if (rd->bof)
      {
	 rd->eof = 1;
	 if ((er = dbf_rawgoto(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
      }
      return 0;
   }
   return 0;
}
