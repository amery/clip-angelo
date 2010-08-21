static int
dbf_getmemoid(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, unsigned int *memoid, unsigned short int *vlen, const char *__PROC__)
{
   char memo[11];

   int er;

   *memoid = 0;
   if (strchr("MPG", rd->fields[no].type))
   {
      if (rd->sig != 0x30)
      {
	 if ((er = rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 10, memo, __PROC__)))
	    return er;
	 memo[10] = 0;
	 *memoid = atoi(memo);
      }
      else
      {
	 if ((er = rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 4, memo, __PROC__)))
	    return er;
	 *memoid = _rdd_uint((unsigned char *) memo);
      }
   }
   else if (rd->fields[no].type == 'V')
   {
      if ((er = rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs + rd->fields[no].len - 6, 4, memo, __PROC__)))
	 return er;
      *memoid = _rdd_uint((unsigned char *) memo);
      if (vlen)
      {
	 if ((er = rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs + rd->fields[no].len - 2, 2, memo, __PROC__)))
	    return er;
	 *vlen = _rdd_ushort((unsigned char *) memo);
      }
   }
   else
   {
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   }
   return 0;
}
