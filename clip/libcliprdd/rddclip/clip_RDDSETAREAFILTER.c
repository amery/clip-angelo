int
clip_RDDSETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETAREAFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int h = _clip_parni(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   fp->active = 1;
   if (rd->filter)
      rd->filter->active = 0;
   rd->filter = fp;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
