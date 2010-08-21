int
rm_evalpartial(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, ClipVar * block, unsigned int *ret, unsigned int *cnt, const char *__PROC__)
{
   ClipVar v;

   unsigned int oldrecno = fp->rd->recno;

   int oldbof = fp->rd->bof;

   int oldeof = fp->rd->eof;

   int recno;

   *ret = 0;
   if (cnt)
      *cnt = 0;

   if (!fp->rmap && cnt)
   {
      if (fp->list)
	 *cnt = fp->listlen;
      else if (rdd_lastrec(ClipMachineMemory, fp->rd, (int *) cnt, __PROC__))
	 goto err;
      return 0;
   }

   memset(&v, 0, sizeof(ClipVar));
   fp->rd->bof = fp->rd->eof = 0;
   for (recno = 1; recno <= fp->size; recno++)
   {
      if (_rm_getbit(fp->rmap, fp->size, recno))
      {
	 if (fp->optimize == 2)
	 {
	    if (cnt)
	       (*cnt)++;
	 }
	 else
	 {
	    int fok;

	    if (fp->rd->vtbl->rawgo(ClipMachineMemory, fp->rd, recno, 0, __PROC__))
	       goto err;
	    if (rdd_childs(ClipMachineMemory, fp->rd, __PROC__))
	       return 1;
	    if (block)
	    {
	       ClipVar v, *vp;

	       if (rdd_calc(ClipMachineMemory, fp->rd->area, block, &v, 0))
		  goto err;
	       vp = _clip_vptr(&v);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	       {
		  _clip_destroy(ClipMachineMemory, &v);
		  rdd_err(ClipMachineMemory, EG_SYNTAX, 0, __FILE__, __LINE__, __PROC__, "Codeblock returns non-logical value");
		  goto err;
	       }
	       fok = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	       _clip_destroy(ClipMachineMemory, &v);
	    }
	    else
	    {
	       if (rdd_calcfilter(ClipMachineMemory, fp->rd, &fok, __PROC__))
		  return 1;
	    }
	    if (!fok)
	       _rm_clrbit(fp->rmap, fp->size, fp->rd->recno);
	    else if (cnt)
	       (*cnt)++;
	 }
	 (*ret)++;
      }
   }
   if (fp->rd->vtbl->rawgo(ClipMachineMemory, fp->rd, oldrecno, 0, __PROC__))
      goto err;
   fp->rd->bof = oldbof;
   fp->rd->eof = oldeof;
   if (rdd_childs(ClipMachineMemory, fp->rd, __PROC__))
      return 1;
   return 0;
 err:
   return 1;
}
