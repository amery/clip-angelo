static int
fpt_info(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, int cmd, const char *__PROC__)
{
   switch (cmd)
   {
   case DBI_MEMOBLOCKSIZE:
      {
	 _clip_retni(ClipMachineMemory, rm->blocksize);
	 break;
      }
   }
   return 0;
}
