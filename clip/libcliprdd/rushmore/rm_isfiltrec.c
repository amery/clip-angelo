int
rm_isfiltrec(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, unsigned int rec, int *r, const char *__PROC__)
{
   *r = 0;
   if (fp->rmap && _rm_getbit(fp->rmap, fp->size, rec))
   {
      unsigned int oldrecno = fp->rd->recno;

      fp->rd->recno = rec;
      if (rdd_calcfilter(ClipMachineMemory, fp->rd, r, __PROC__))
	 return 1;
      fp->rd->recno = oldrecno;
   }
   return 0;
}
