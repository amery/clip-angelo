static int
dbf_setmemoid(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, unsigned int memoid, const char *__PROC__)
{
   char memo[11];

   int er;

   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   rd->updated = 1;
   if (strchr("MPG", rd->fields[no].type))
   {
      if (rd->sig != 0x30)
      {
	 snprintf(memo, sizeof(memo), "%d", memoid);
	 if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 10, memo, __PROC__)))
	    return er;
      }
      else
      {
	 _rdd_put_uint((unsigned char *) memo, memoid);
	 if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 4, memo, __PROC__)))
	    return er;
      }
   }
   else if (rd->fields[no].type == 'V')
   {
      _rdd_put_uint((unsigned char *) memo, memoid);
      if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs + rd->fields[no].len - 6, 4, memo, __PROC__)))
	 return er;
   }
   else
   {
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
   }
   return 0;
}
