static int
dbf_deleted(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *deleted, const char *__PROC__)
{
   int er;

   *deleted = 0;
   if (rd->recno == 0)
      return 0;
   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;
   *deleted = (((char *) rd->record)[0] != 0x20);
   return 0;
}
