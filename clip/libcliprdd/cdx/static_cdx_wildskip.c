static int
cdx_wildskip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *pattern, int regular, const char *s, ClipVar * block, int *found, const char *__PROC__)
{
   CDX_HEADER hdr;

   CDX_PAGE page;

   CDX_LEAF *leaf = (CDX_LEAF *) & page;

   ClipVar v, *vp, r;

   char *key = calloc(1, ro->bufsize + 1);

   char *lockey = calloc(1, ro->bufsize + 1);

   char *p = (char *) (leaf) + sizeof(CDX_LEAF);

   unsigned long long item;

   unsigned int rec = 0;

   int dup = 0, trail = 0;

   int ok, fok;

   int fnd, out = 0, i, er;

#ifdef HAVE_REGEX_H
   regex_t reg;
#endif

   *found = 1;
#ifdef HAVE_REGEX_H
   if (regular)
      regcomp(&reg, pattern, REG_EXTENDED | REG_NOSUB);
#else
   regular = 0;
#endif
   if (rd->shared || !ro->valid_stack)
   {
      char cntcdx[4];

      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	 return er;
      if (!ro->valid_stack || ro->cntcdx != _rdd_backuint((unsigned char *) cntcdx))
      {
	 if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	    goto err;
	 vp = _clip_vptr(&v);
	 if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	    goto err;
	 _clip_destroy(ClipMachineMemory, &v);
	 ro->curoffs = 0;

	 _CDX_HEADER(ro, &hdr);
	 ro->stack[0].page = _rdd_uint(hdr.root);
	 ro->level = 0;

	 if (!ro->descend)
	 {
	    if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &fnd, NULL, __PROC__)))
	       goto err;
	 }
	 else
	 {
	    if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &fnd, NULL, __PROC__)))
	       goto err;
	 }
	 _clip_destroy(ClipMachineMemory, &v);
      }
   }

   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
      goto err;

   for (i = 0; i <= ro->stack[ro->level].pos; i++)
   {
      memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
      _CDX_LEAF_ITEM(item, rec, dup, trail);
      p -= ro->bufsize - (dup + trail);
      memcpy(key + dup, p, ro->bufsize - (dup + trail));
   }
   memset(key + dup + ro->bufsize - (dup + trail), (int) (block ? 0x20 : 0x00), trail);

   if (block)
      memset(&v, 0, sizeof(ClipVar));

   *found = 0;
   while (!out && ((block || regular) ? 1 : (memcmp(key, pattern, s - pattern) == 0)))
   {
      if (!block)
      {
	 if (regular)
	 {
#ifdef HAVE_REGEX_H
	    *found = !regexec(&reg, key, 0, NULL, 0);
#else
	    *found = 0;
#endif
	 }
	 else
	 {
	    *found = (_clip_glob_match(key + (s - pattern), s, 1) != -1);
	 }
	 if (*found)
	 {
	    if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rec, 1, __PROC__)))
	       goto err;
	    if (!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG))
	       break;
	    if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	       goto err;
	    if (ok > 0)
	    {
	       *found = 0;
	       break;
	    }
	    if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	       goto err;
	    if (!ok && fok)
	       break;
	 }
      }
      else
      {
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rec, 1, __PROC__)))
	    goto err;
	 ok = 0;
	 fok = 1;
	 if (!(!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG)))
	 {

	    if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	       goto err;
	    if (ok > 0)
	    {
	       *found = 0;
	       break;
	    }
	    if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	       goto err;
	 }
	 if (!ok && fok)
	 {
	    _cdx_keyvar(ro, key, lockey, &v);
	    if ((er = _clip_eval(ClipMachineMemory, block, 1, &v, &r)))
	       goto err;
	    *found = r.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	    if (*found)
	       break;
	 }
      }
      ro->stack[ro->level].pos++;
      if (ro->stack[ro->level].pos >= _rdd_ushort(leaf->nkeys))
      {
	 if (_rdd_uint(leaf->right) != 0xffffffff)
	 {
	    ro->stack[ro->level].page = _rdd_uint(leaf->right);
	    if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	       goto err;
	    p = (char *) leaf + sizeof(CDX_LEAF);
	    memcpy(&item, leaf->keys, leaf->bytes);
	    _CDX_LEAF_ITEM(item, rec, dup, trail);
	    p -= ro->bufsize - (dup + trail);
	    memcpy(key + dup, p, ro->bufsize - (dup + trail));
	    memset(key + dup + ro->bufsize - (dup + trail), (int) (block ? 0x20 : 0x00), trail);

	    if (_rdd_ushort(leaf->nkeys) == 0)
	       return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_nokeys);
	    ro->stack[ro->level].pos = 0;
	 }
	 else
	 {
	    out = 1;
	 }
      }
      else
      {
	 memcpy(&item, leaf->keys + ro->stack[ro->level].pos * leaf->bytes, leaf->bytes);
	 _CDX_LEAF_ITEM(item, rec, dup, trail);
	 p -= ro->bufsize - (dup + trail);
	 memcpy(key + dup, p, ro->bufsize - (dup + trail));
	 memset(key + dup + ro->bufsize - (dup + trail), (int) (block ? 0x20 : 0x00), trail);
      }
      *found = 0;
   }
   if (out)
   {
      *found = 0;
      ro->valid_stack = 0;
   }
   else
   {
      ro->valid_stack = 1;
   }

#ifdef HAVE_REGEX_H
   if (regular)
      regfree(&reg);
#endif
   free(lockey);
   free(key);
   return 0;
 err:
#ifdef HAVE_REGEX_H
   if (regular)
      regfree(&reg);
#endif
   free(lockey);
   free(key);
   return er;
}
