int
rdd_keyadd(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   if (!ro->custom)
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, "Trying to add key to automatic (non-custom) order");
   return ro->vtbl->addkey(ClipMachineMemory, rd, ro, v, __PROC__);
}
