int
cdx_calcfiltlist(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, RDD_FILTER * fp, const char *__PROC__)
{
   int i, er;

   BTREE *bt;

   void *key = malloc(sizeof(unsigned int) + ro->bufsize);

   ClipVar vv, *vp;

   if (fp->list)
   {
      bt = bt_create(0, fp->listlen, sizeof(unsigned int) + ro->bufsize, _cdx_compare);
      for (i = 0; i < fp->listlen; i++)
      {
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, fp->list[i], 0, __PROC__)))
	    return er;
	 if (ro->simpexpr)
	 {
	    if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
	       return er;
	    vp = &vv;
	 }
	 else
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	       return er;
	    vp = _clip_vptr(&vv);
	 }
	 memcpy(key, &rd->recno, sizeof(unsigned int));
	 if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key + sizeof(unsigned int), __PROC__)))
	    return er;
	 _clip_destroy(ClipMachineMemory, &vv);
	 bt_add(bt, ro, key);
      }
      free(fp->list);
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
      {
	 free(key);
	 return 0;
      }
#else
      fp->listlen = 0;
      for (i = 1; i <= fp->size; i++)
	 if (_rm_getbit(fp->rmap, fp->size, i))
	    fp->listlen++;
#endif
      bt = bt_create(0, fp->listlen, sizeof(unsigned int) + ro->bufsize, _cdx_compare);
#if 1
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
		     {
			if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, t, 0, __PROC__)))
			   return er;
			if (ro->simpexpr)
			{
			   if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
			      return er;
			   vp = &vv;
			}
			else
			{
			   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
			      return er;
			   vp = _clip_vptr(&vv);
			}
			memcpy(key, &rd->recno, sizeof(unsigned int));
			if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key + sizeof(unsigned int), __PROC__)))
			   return er;
			_clip_destroy(ClipMachineMemory, &vv);
			bt_add(bt, ro, key);
		     }
		  }
	       }
	    }
	 }
      }
#else
      for (i = 1; i <= fp->size; i++)
      {
	 if (_rm_getbit(fp->rmap, fp->size, i))
	 {
	    if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, i, 0, __PROC__)))
	       return er;
	    if (ro->simpexpr)
	    {
	       if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
		  return er;
	       vp = &vv;
	    }
	    else
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	    }
	    memcpy(key, &rd->recno, sizeof(unsigned int));
	    if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key + sizeof(unsigned int), __PROC__)))
	       return er;
	    _clip_destroy(ClipMachineMemory, &vv);
	    bt_add(bt, ro, key);
	 }
      }
#endif
   }
   fp->list = malloc(sizeof(unsigned int) * (fp->listlen + 1));
   if (!ro->descend)
   {
      bt_first(bt);
      if (bt_key(bt))
      {
	 i = 0;
	 fp->list[i] = *(unsigned int *) bt_key(bt);
	 while (!bt_next(bt))
	 {
	    i++;
	    fp->list[i] = *(unsigned int *) bt_key(bt);
	 }
      }
   }
   else
   {
      bt_last(bt);
      if (bt_key(bt))
      {
	 i = 0;
	 fp->list[i] = *(unsigned int *) bt_key(bt);
	 while (!bt_prev(bt))
	 {
	    i++;
	    fp->list[i] = *(unsigned int *) bt_key(bt);
	 }
      }
   }
   bt_destroy(bt);
   free(key);
   return 0;
}
