int
clip_M6_ISFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ISFILTER";

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
   {
      fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   }
   else
   {
      DBWorkArea *wa = cur_area(ClipMachineMemory);

      if (!wa)
      {
	 er = rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
	 goto err;
      }
      fp = wa->rd->filter;
   }
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }

   _clip_retni(ClipMachineMemory, fp->optimize);
   return 0;
 err:
   return er;
}
