int
rdd_ii_skip(ClipMachine * ClipMachineMemory, int h, int recs, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   int i;

   int er;

   CHECKIO(ro);

   if (recs > 0)
   {
      for (i = 0; i < recs; i++)
      {
	 if ((er = ro->vtbl->ii_next(ClipMachineMemory, ro, __PROC__)))
	    return er;
      }
   }
   else
   {
      for (i = 0; i > recs; i--)
      {
	 if ((er = ro->vtbl->ii_prev(ClipMachineMemory, ro, __PROC__)))
	    return er;
      }
   }
   return 0;
}
