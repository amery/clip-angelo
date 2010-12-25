int
clip_RDDCLOSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLOSEAREA";

   int key = _clip_parni(ClipMachineMemory, 1);

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int er;

   if (!rd)
      return EG_NOTABLE;

   if (rd->area != -1)
      return 0;
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if (rd->readonly)
      rd->vtbl->_rlock(ClipMachineMemory, rd, __PROC__);
   else
      rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__);
   if ((er = rdd_closearea(ClipMachineMemory, rd, __PROC__)))
      goto err;
   _clip_destroy_c_item(ClipMachineMemory, key, _C_ITEM_TYPE_RDD);
   return 0;
 err:
   return er;
}
