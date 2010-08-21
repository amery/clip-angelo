int
clip_M6_FILTGOREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTGOREC";

   int h = _clip_parni(ClipMachineMemory, 1);

   int recno = _clip_parni(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (fp->optimize)
      fp->recno = recno;
   _clip_retni(ClipMachineMemory, fp->recno);
   return 0;
 err:
   return er;
}
