int
rm_inverse(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, const char *__PROC__)
{
   int i;

   unsigned int bytes = ((fp->size + 1) >> 5) + 1;

   if (fp->rmap)
   {
      for (i = 0; i < bytes; i++)
      {
	 fp->rmap[i] = ~fp->rmap[i];
      }
   }
   if (fp->sfilter)
   {
      char *tmp = strdup(fp->sfilter);

      fp->sfilter = realloc(fp->sfilter, strlen(fp->sfilter) + 4);
      sprintf(fp->sfilter, "!(%s)", tmp);
      free(tmp);
   }
   fp->fps->op = 4;
   return 0;
}
