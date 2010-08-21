static int
cdx_ii_open(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *__PROC__)
{
   ri->loc = dbf_get_locale(ClipMachineMemory);
   ri->orders = malloc(0);
   ri->norders = 0;
   return 0;
}
