static int
dbf_info(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int cmd, const char *__PROC__)
{
   switch (cmd)
   {
   case DBI_ISDBF:
      {
	 _clip_retl(ClipMachineMemory, 1);
	 break;
      }
   case DBI_LOCKOFFSET:
      {
	 _clip_retni(ClipMachineMemory, rd->lockstyle ? 0x10000000 : 1000000000);
	 break;
      }
   case DBI_VALIDBUFFER:
      {
	 _clip_retl(ClipMachineMemory, 1);
	 break;
      }
   }
   return 0;
}
