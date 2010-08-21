int
_clip_rddfieldno(ClipMachine * ClipMachineMemory, int h, long hash)
{
   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RDD);

   if (!rd)
      return -1;
   return _rdd_fieldno(rd, hash);
}
