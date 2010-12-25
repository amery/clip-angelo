static int
dbt_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, const char *__PROC__)
{
   rm->blocksize = 512;
   return 0;
}
