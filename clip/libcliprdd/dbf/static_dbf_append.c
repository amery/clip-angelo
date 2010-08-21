static int
dbf_append(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   char recs[4];

   int lastrec, flen, i, er;

   if (rd->readonly)
   {
      er = rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
      goto err;
   }
   if ((er = dbf_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      goto err;
   rd->recno = lastrec + 1;
   if (rd->curord >= 0)
      rd->orders[rd->curord]->valid_stack = 0;
   rd->updated = 1;

   if (!rd->shared)
   {
      flen = rd->hdrsize + rd->recsize * (lastrec + 1) + 1;
      if (rd->file.md != (caddr_t) - 1)
      {
	 if (flen > rd->file.mapsize)
	 {
	    int delta = lastrec / 5;

	    if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (lastrec + 1 + delta) - 1, 1, "", __PROC__)))
	       goto err;
	 }
      }
   }

   if (rd->sig != 0x30)
   {
      memset(rd->record, 32, rd->recsize);
      for (i = 0; i < rd->nfields; i++)
	 if (rd->fields[i].type == 'V' || rd->fields[i].type == 'X')
	    memset(rd->record + rd->fields[i].offs, 0, rd->fields[i].len);
   }
   ((char *) rd->record)[0] = 0x20;
   ((char *) rd->record)[rd->recsize] = 0x1a;
   rd->valid = 1;
   rd->changed = 0;
   lastrec++;
   _rdd_put_uint((unsigned char *) recs, lastrec);
   if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (lastrec - 1), rd->recsize + 1, rd->record, __PROC__)))
      goto err;
   if ((er = rdd_write(ClipMachineMemory, &rd->file, 4, 4, recs, __PROC__)))
      goto err;
   rd->bof = rd->v_bof = rd->eof = 0;
   return 0;
 err:
   return er;
}
