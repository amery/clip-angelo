int
clip_M6_FILTSKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTSKIP";

   int h = _clip_parni(ClipMachineMemory, 1);

   int nrecs = _clip_parni(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   int i, er, recno = 0;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (fp->optimize)
   {
      recno = fp->recno;
      if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
	 nrecs = 1;
      if (nrecs < 0)
      {
	 if (recno > fp->size)
	    recno = fp->size + 1;
	 for (i = 0; i > nrecs && recno > 0; i--)
	 {
	    while (--recno > 0 && !_rm_getbit(fp->rmap, fp->size, recno));
	 }
	 fp->recno = recno;
      }
      else
      {
	 if (recno < 0)
	    recno = 0;
	 for (i = 0; i < nrecs && recno <= fp->size; i++)
	 {
	    while (++recno <= fp->size && !_rm_getbit(fp->rmap, fp->size, recno));
	 }
	 fp->recno = recno;
	 if (recno > fp->size)
	    recno = 0;
      }
   }
   _clip_retni(ClipMachineMemory, recno);
   return 0;
 err:
   return er;
}
