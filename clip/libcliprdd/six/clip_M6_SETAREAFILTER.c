int
clip_M6_SETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_SETAREAFILTER";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int h = _clip_parni(ClipMachineMemory, 1);

   char expr[PATH_MAX];

   RDD_FILTER *fp;

   int er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
   {
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   fp->active = 1;
   if (wa->rd->filter)
      wa->rd->filter->active = 0;
   wa->rd->filter = fp;

   fp->rd = wa->rd;
   if (!fp->custom)
   {
      rdd_expandmacro(wa->rd->area, 0, fp->sfilter, expr);
      if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &fp->fps->bfilter)))
	 goto err;
   }

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
