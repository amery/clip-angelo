int
clip_RDD_M6_SETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_SETAREAFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int h = _clip_parni(ClipMachineMemory, 2);

   char expr[PATH_MAX];

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

   fp->rd = rd;
   if (!fp->custom)
   {
      rdd_expandmacro(rd->area, 0, fp->sfilter, expr);
      if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &fp->fps->bfilter)))
	 goto err;
   }

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
