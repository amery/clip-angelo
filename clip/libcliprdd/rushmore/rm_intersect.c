int
rm_intersect(ClipMachine * ClipMachineMemory, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__)
{
   int i;

   unsigned int bytes = ((f1->size + 1) >> 5) + 1;

   if (f1->rmap)
   {
      for (i = 0; i < bytes; i++)
      {
	 f1->rmap[i] &= f2->rmap[i];
      }
   }
   if (f1->sfilter && f2->sfilter)
   {
      char *tmp = strdup(f1->sfilter);

      f1->sfilter = realloc(f1->sfilter, strlen(f1->sfilter) + strlen(f2->sfilter) + 12);
      sprintf(f1->sfilter, "(%s) .AND. (%s)", tmp, f2->sfilter);
      free(tmp);
   }
   else
   {
      if (f1->sfilter)
	 free(f1->sfilter);
      f1->sfilter = NULL;
   }
   f1->nfps++;
   f1->fps = (RDD_FPS *) realloc(f1->fps, f1->nfps * sizeof(RDD_FPS));
   _clip_clone(ClipMachineMemory, &f1->fps[f1->nfps - 1].bfilter, &f2->fps->bfilter);
   f1->fps[f1->nfps - 1].op = 2;

   f1->optimize += f1->optimize;
   if (f1->optimize == 4)
      f1->optimize = 2;
   else if (f1->optimize > 0)
      f1->optimize = 1;
   return 0;
}
