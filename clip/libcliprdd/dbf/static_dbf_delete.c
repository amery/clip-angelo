static int
dbf_delete(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   char d = 0x2a;

   int er;

   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;
   rd->updated = rd->changed = 1;
   ((char *) rd->record)[0] = d;
   return 0;
}
