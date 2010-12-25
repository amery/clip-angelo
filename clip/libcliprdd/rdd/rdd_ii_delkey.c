int
rdd_ii_delkey(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   if (ro->eof)
      return 0;
   return ro->vtbl->ii_delkey(ClipMachineMemory, ro, __PROC__);
}
