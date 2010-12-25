int
clip_M6_FILTINVERSE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTINVERSE";

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   unsigned int ret;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if ((er = fp->rd->vtbl->_rlock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   if ((er = rm_evalpartial(ClipMachineMemory, fp, NULL, &ret, NULL, __PROC__)))
      goto err_unlock;
   if ((er = fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   if ((er = rm_inverse(ClipMachineMemory, fp, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err_unlock:
   fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__);
 err:
   return er;
}
