int
clip_M6_FREEFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FREEFILTER";

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (fp->active)
   {
      _clip_retl(ClipMachineMemory, 0);
   }
   else
   {
      if ((er = rdd_destroyfilter(ClipMachineMemory, fp, __PROC__)))
	 goto err;
      _clip_retl(ClipMachineMemory, 1);
   }
   return 0;
 err:
   return er;
}
