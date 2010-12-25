static int
_cdx_addstick(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, void *key1, unsigned int rec1, unsigned int page1, void *key2, unsigned int rec2, unsigned int page2, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int s = (ro->bufsize + 8) * ro->stack[level].pos;

   int fr, er;

   int i = ro->stack[level].pos + 1;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   memcpy(branch->keys + s, key1, ro->bufsize);
   _rdd_put_backuint(branch->keys + s + ro->bufsize, rec1);
   _rdd_put_backuint(branch->keys + s + ro->bufsize + 4, page1);

   fr = sizeof(branch->keys) - (ro->bufsize + 8) * _rdd_ushort(branch->nkeys);

   if (fr > ro->bufsize + 8)
   {
      /* add */
      memmove(branch->keys + (ro->bufsize + 8) * (i + 1), branch->keys + (ro->bufsize + 8) * i, (_rdd_ushort(branch->nkeys) - i) * (ro->bufsize + 8));
      memcpy(branch->keys + (ro->bufsize + 8) * i, key2, ro->bufsize);
      _rdd_put_backuint(branch->keys + (ro->bufsize + 8) * i + ro->bufsize, rec2);
      _rdd_put_backuint(branch->keys + (ro->bufsize + 8) * i + ro->bufsize + 4, page2);
      _rdd_put_ushort(branch->nkeys, _rdd_ushort(branch->nkeys) + 1);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
	 return er;
      if (level && (i == _rdd_ushort(branch->nkeys) - 1))
      {
	 if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, level - 1, key2, rec2, __PROC__)))
	    return er;
      }
   }
   else
   {
      CDX_BRANCH new;

      unsigned int newpage;

      void *keys = malloc((_rdd_ushort(branch->nkeys) + 1) * ro->bufsize);

      unsigned int *recnos = malloc((_rdd_ushort(branch->nkeys) + 1) * sizeof(unsigned int));

      unsigned int *pages = malloc((_rdd_ushort(branch->nkeys) + 1) * sizeof(unsigned int));

      int j;

      int c1 = (_rdd_ushort(branch->nkeys) + 1) / 2;

      int c2 = (_rdd_ushort(branch->nkeys) + 1) - c1;

      if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &newpage, __PROC__)))
	 goto err1;

      for (j = 0; j < _rdd_ushort(branch->nkeys); j++)
      {
	 s = j * (ro->bufsize + 8);
	 memcpy(keys + j * ro->bufsize, branch->keys + s, ro->bufsize);
	 recnos[j] = _rdd_backuint(branch->keys + s + ro->bufsize);
	 pages[j] = _rdd_backuint(branch->keys + s + ro->bufsize + 4);
      }
      memmove(keys + (i + 1) * ro->bufsize, keys + i * ro->bufsize, (_rdd_ushort(branch->nkeys) - i) * ro->bufsize);
      memmove(&recnos[i + 1], &recnos[i], (_rdd_ushort(branch->nkeys) - i) * sizeof(unsigned int));
      memmove(&pages[i + 1], &pages[i], (_rdd_ushort(branch->nkeys) - i) * sizeof(unsigned int));

      memcpy(keys + i * ro->bufsize, key2, ro->bufsize);
      recnos[i] = rec2;
      pages[i] = page2;

      memset(branch->keys, 0, sizeof(branch->keys));
      memset(new.keys, 0, sizeof(new.keys));
      _rdd_put_ushort(branch->attrs, 0);
      _rdd_put_ushort(new.attrs, 0);
      _rdd_put_ushort(branch->nkeys, c1);
      _rdd_put_ushort(new.nkeys, c2);
      _rdd_put_uint(new.right, _rdd_uint(branch->right));
      _rdd_put_uint(branch->right, newpage);
      _rdd_put_uint(new.left, ro->stack[level].page);
      if (_rdd_uint(new.right) != 0xffffffff)
      {
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(new.right) + 4, 4, &newpage, __PROC__)))
	    goto err1;
      }
      for (j = 0; j < c1; j++)
      {
	 s = j * (ro->bufsize + 8);
	 memcpy(branch->keys + s, keys + j * ro->bufsize, ro->bufsize);
	 _rdd_put_backuint(branch->keys + s + ro->bufsize, recnos[j]);
	 _rdd_put_backuint(branch->keys + s + ro->bufsize + 4, pages[j]);
      }
      for (; j < c1 + c2; j++)
      {
	 s = (j - c1) * (ro->bufsize + 8);
	 memcpy(new.keys + s, keys + j * ro->bufsize, ro->bufsize);
	 _rdd_put_backuint(new.keys + s + ro->bufsize, recnos[j]);
	 _rdd_put_backuint(new.keys + s + ro->bufsize + 4, pages[j]);
      }
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
	 goto err1;
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, newpage, sizeof(CDX_BRANCH), &new, __PROC__)))
	 goto err1;

      if (level == 0)
      {
	 unsigned int brpage;

	 if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &brpage, __PROC__)))
	    goto err1;

	 if ((er = _cdx_addroot(ClipMachineMemory, ro, brpage,
				branch->keys + (c1 - 1) * (ro->bufsize + 8),
				_rdd_backuint(branch->keys +
					      (c1 - 1) * (ro->bufsize +
							  8) +
					      ro->bufsize),
				ro->stack[level].page, new.keys + (c2 - 1) * (ro->bufsize + 8), _rdd_backuint(new.keys + (c2 - 1) * (ro->bufsize + 8) + ro->bufsize), newpage, __PROC__)))
	    goto err1;
      }
      else
      {
	 if ((er = _cdx_addstick(ClipMachineMemory, ro, level - 1,
				 branch->keys + (c1 - 1) * (ro->bufsize +
							    8),
				 _rdd_backuint(branch->keys +
					       (c1 - 1) * (ro->bufsize +
							   8) +
					       ro->bufsize),
				 ro->stack[level].page, new.keys + (c2 - 1) * (ro->bufsize + 8), _rdd_backuint(new.keys + (c2 - 1) * (ro->bufsize + 8) + ro->bufsize), newpage, __PROC__)))
	    goto err1;
      }
      free(keys);
      free(recnos);
      free(pages);
      goto cont;
    err1:
      free(keys);
      free(recnos);
      free(pages);
      return er;
   }
 cont:
   return 0;
}
