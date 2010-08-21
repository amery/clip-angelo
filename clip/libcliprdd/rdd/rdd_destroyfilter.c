int
rdd_destroyfilter(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, const char *__PROC__)
{
   int i;

   for (i = 0; i < fp->nfps; i++)
      _clip_destroy(ClipMachineMemory, &fp->fps[i].bfilter);
   _clip_destroy_c_item(ClipMachineMemory, fp->handle, _C_ITEM_TYPE_RYO);
   return 0;
}
