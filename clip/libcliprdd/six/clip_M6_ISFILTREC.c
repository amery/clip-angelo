int
clip_M6_ISFILTREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ISFILTREC";

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   unsigned int rec = _clip_parni(ClipMachineMemory, 2);

   int r, er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }

   if ((er = rm_isfiltrec(ClipMachineMemory, fp, rec, &r, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}
