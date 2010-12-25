int
clip_M6_EVALPARTIAL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_EVALPARTIAL";

   int h = _clip_parni(ClipMachineMemory, 1);

   ClipVar *block = _clip_spar(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   unsigned int ret;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   if (block && (block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType))
      block = NULL;

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if ((er = fp->rd->vtbl->_rlock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   if ((er = rm_evalpartial(ClipMachineMemory, fp, block, &ret, NULL, __PROC__)))
      goto err_unlock;
   if ((er = fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err_unlock:
   fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__);
 err:
   return er;
}
