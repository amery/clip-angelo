int
rdd_ii_gotop(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   return ro->vtbl->ii_gotop(ClipMachineMemory, ro, __PROC__);
}
