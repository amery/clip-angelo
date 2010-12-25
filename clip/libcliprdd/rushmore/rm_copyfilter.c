int
rm_copyfilter(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, RDD_FILTER ** ret, const char *__PROC__)
{
   RDD_FILTER *r = calloc(sizeof(RDD_FILTER), 1);

   int i;

   r->custom = fp->custom;
   r->optimize = fp->optimize;
   r->nfps = fp->nfps;
   r->fps = calloc(fp->nfps, sizeof(RDD_FPS));
   for (i = 0; i < fp->nfps; i++)
   {
      _clip_clone(ClipMachineMemory, &r->fps[i].bfilter, &fp->fps[i].bfilter);
      r->fps[i].op = fp->fps[i].op;
   }
   if (fp->sfilter)
      r->sfilter = strdup(fp->sfilter);
   r->size = fp->size;
   if (fp->rmap)
   {
      int i;

      int bytes = ((r->size + 1) >> 5) + 1;

      r->rmap = calloc(sizeof(unsigned int), bytes);
      for (i = 0; i < bytes; i++)
	 r->rmap[i] = fp->rmap[i];
   }
   r->rd = fp->rd;
   r->handle = _clip_store_c_item(ClipMachineMemory, r, _C_ITEM_TYPE_RYO, destroy_ryo);
   *ret = r;
   return 0;
}
