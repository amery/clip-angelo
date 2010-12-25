static int
_cdx_back_search_tree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, void *key, int len, unsigned int recno, int *found, int *out, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int er, i, ll = 0;

   *found = 0;
   if (out)
      *out = 0;
   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   while (!leaf)
   {
      for (i = _rdd_ushort(branch->nkeys) - 1; i >= 0; i--)
      {
	 unsigned int recno1;

	 int s;

	 recno1 = _rdd_backuint(branch->keys + i * (ro->bufsize + 8) + ro->bufsize);
	 s = _cdx_cmp(ro, branch->keys + i * (ro->bufsize + 8), key, len);
	 if (recno && !s)
	    s = recno1 - recno;
	 if ((s <= 0) || (i == 0))
	 {
	    ro->stack[ro->level].pos = i;
	    if ((s <= 0) && (i < _rdd_ushort(branch->nkeys) - 1))
	    {
	       ro->stack[ro->level].pos++;
	       (ro->level)++;
	       if ((ro->level) >= MAX_BTREE_DEEP)
		  return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
	       if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
		  return er;
	       _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
	       break;
/*					if(leaf){
						void* val = calloc(1,ro->bufsize);
						unsigned long long item;
						unsigned int rec;
						int dup,trail;
						char* p = (char*)leaf+sizeof(CDX_LEAF);

						memcpy(&item,leaf->keys,leaf->bytes);
						_CDX_LEAF_ITEM(item,rec,dup,trail);
						p -= ro->bufsize-(dup+trail);
						memcpy(val,p,ro->bufsize-trail);
						s = _cdx_cmp(ro,val,key,len);
						free(val);
						if(recno && !s)
							s = rec-recno;
						if(s > 0){
							ro->level--;
							ro->stack[ro->level].pos = i;
							ll = 1;
						} else {
							break;
						}
					} else {
						break;
					}*/
	    }
	    (ro->level)++;
	    if ((ro->level) >= MAX_BTREE_DEEP)
	       return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
	    if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
	       return er;
	    _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
	    break;
	 }
      }
   }
   if (recno)
   {
      *found = _cdx_search_leaf_recno(ro, leaf, recno, &(ro->stack[ro->level].pos));
   }
   else
   {
      int o;

      *found = _cdx_search_leaf(ro, leaf, key, len, 0, &(ro->stack[ro->level].pos), 1);
      if (ro->stack[ro->level].pos < 0)
      {
	 ro->stack[ro->level].pos = 0;
	 if ((er = _cdx_prev_rm(ClipMachineMemory, ro, leaf, &o, 0, __PROC__)))
	    return er;
	 if (out && o)
	 {
	    *out = -1;
	 }
	 else
	 {
	    void *val = calloc(1, ro->bufsize);

	    unsigned int recno;

	    int s;

	    _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &recno, val);
	    s = _cdx_cmp(ro, val, key, len);
	    free(val);
	    if (!s)
	       *found = 1;
/*
				void* val = calloc(1,ro->bufsize);
				unsigned long long item;
				unsigned int rec;
				int dup,trail,s;
				char* p = (char*)leaf+sizeof(CDX_LEAF);

				memcpy(&item,leaf->keys,leaf->bytes);
				_CDX_LEAF_ITEM(item,rec,dup,trail);
				p -= ro->bufsize-(dup+trail);
				memcpy(val,p,ro->bufsize-trail);
				s = _cdx_cmp(ro,val,key,len);
				free(val);
				if(recno && !s)
					s = rec-recno;
				if(!s)
					*found = 1;
*/
	 }
      }
      if ((ro->stack[ro->level].pos >= _rdd_ushort(leaf->nkeys)) && !ll)
      {
	 ro->stack[ro->level].pos = _rdd_ushort(leaf->nkeys);
	 if ((er = _cdx_next_rm(ClipMachineMemory, ro, leaf, &o, 0, __PROC__)))
	    return er;
	 if (out && o)
	 {
	    *out = 1;
	 }
	 else
	 {
	    void *val = calloc(1, ro->bufsize);

	    unsigned long long item;

	    unsigned int rec;

	    int dup, trail, s;

	    char *p = (char *) leaf + sizeof(CDX_LEAF);

	    memcpy(&item, leaf->keys, leaf->bytes);
	    _CDX_LEAF_ITEM(item, rec, dup, trail);
	    p -= ro->bufsize - (dup + trail);
	    memcpy(val, p, ro->bufsize - trail);
	    s = _cdx_cmp(ro, val, key, len);
	    free(val);
	    if (recno && !s)
	       s = rec - recno;
	    if (!s)
	       *found = 1;
	 }
      }
   }
   return 0;
}
