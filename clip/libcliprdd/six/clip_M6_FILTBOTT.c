int
clip_M6_FILTBOTT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTBOTT";

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   int er, recno;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   fp->recno = 0;
   recno = fp->size + 1;
   if (fp->optimize)
   {
      while (--recno > 0 && !_rm_getbit(fp->rmap, fp->size, recno));
      fp->recno = recno;
   }
   _clip_retni(ClipMachineMemory, fp->recno);
   return 0;
 err:
   return er;
}
