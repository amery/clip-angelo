static int
ctx_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   int i;

   for (i = 0; i < ri->norders; i++)
      destroy_rdd_order(ClipMachineMemory, ri->orders[i]);
   destroy_rdd_index(ri);
   return 0;
}
