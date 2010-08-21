static int
cdx_descend(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int descend, int *res, const char *__PROC__)
{
   *res = ro->descend;
   if (descend != -1)
      ro->descend = descend;
   return 0;
}
