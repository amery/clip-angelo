static int
ntx_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   destroy_rdd_order(ClipMachineMemory, ri->orders[0]);
   destroy_rdd_index(ri);
   return 0;
}
