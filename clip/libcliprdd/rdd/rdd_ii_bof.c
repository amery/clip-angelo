int
rdd_ii_bof(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   _clip_retl(ClipMachineMemory, ro->bof);
   return 0;
}
