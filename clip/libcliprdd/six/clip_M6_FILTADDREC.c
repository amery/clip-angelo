int
clip_M6_FILTADDREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTADDREC";

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   unsigned int rn = _clip_parni(ClipMachineMemory, 2);

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
   if (fp->custom && fp->rmap && rn <= fp->size)
   {
      _rm_setbit(fp->rmap, fp->size, rn);
      _clip_retl(ClipMachineMemory, 1);
   }
   else
   {
      _clip_retl(ClipMachineMemory, 0);
   }
   return 0;
 err:
   return er;
}
