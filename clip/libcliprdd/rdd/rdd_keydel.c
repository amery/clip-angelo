int
rdd_keydel(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   if (!ro->custom)
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, "Trying to delete key from automatic (non-custom) order");
   return ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__);
}
