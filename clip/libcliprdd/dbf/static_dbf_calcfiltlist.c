static int
dbf_calcfiltlist(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__)
{
   BTREE *bt;

   int i, j;

   unsigned int recno;

   if (fp->list)
   {
      bt = bt_create(0, fp->listlen, sizeof(unsigned int), _dbf_compare);
      for (i = 0; i < fp->listlen; i++)
      {
	 recno = fp->list[i];
	 bt_add(bt, NULL, (void *) &recno);
      }
      free(fp->list);
      fp->list = malloc(sizeof(unsigned int) * fp->listlen);
      bt_first(bt);
      i = 0;
      fp->list[i] = *(unsigned int *) bt_key(bt);
      while (!bt_next(bt))
      {
	 i++;
	 fp->list[i] = *(unsigned int *) bt_key(bt);
      }
      bt_destroy(bt);
   }
   else
   {
#if 1
      unsigned int bytes = ((fp->size + 1) >> 5) + 1;

      int i, b, bb, t, tt;

      fp->listlen = 0;
      for (i = 0; i < bytes; i++)
      {
	 if (fp->rmap[i])
	 {
	    for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
	    {
	       if (((char *) fp->rmap)[b])
	       {
		  for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
		  {
		     if (_rm_getbit(fp->rmap, fp->size, t))
			fp->listlen++;
		  }
	       }
	    }
	 }
      }
      if (fp->listlen > 100)
	 return 0;
#else
      fp->listlen = 0;
      for (i = 1; i <= fp->size; i++)
	 if (_rm_getbit(fp->rmap, fp->size, i))
	    fp->listlen++;
#endif
      fp->list = malloc(sizeof(unsigned int) * (fp->listlen + 1));
#if 1
      for (i = 0, j = 0; i < bytes; i++)
      {
	 if (fp->rmap[i])
	 {
	    for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
	    {
	       if (((char *) fp->rmap)[b])
	       {
		  for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
		  {
		     if (_rm_getbit(fp->rmap, fp->size, t))
			fp->list[j++] = t;
		  }
	       }
	    }
	 }
      }
#else
      for (i = 1; i <= fp->size; i++)
      {
	 if (_rm_getbit(fp->rmap, fp->size, i))
	    fp->list[j++] = i;
      }
#endif
   }
   return 0;
}
