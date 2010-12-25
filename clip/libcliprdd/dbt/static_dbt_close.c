static int
dbt_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, const char *__PROC__)
{
   destroy_rdd_memo(rm);
   return 0;
}
