static int
_ntx_split_node(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, char *kk, int rn, unsigned int cc, const char *__PROC__)
{
   NTX_PAGE rpp, lpp;

   NTX_PAGE *pp;

   int count, mediane, i, j;

   unsigned int lnode, rchild;

   unsigned int rnode = ro->stack[ro->level].page;

   int buckno = ro->stack[ro->level].pos;

   char *pbuf = (char *) malloc(sizeof(NTX_PAGE));

   NTX_BUCKET *bp;

   char key[1024];

   unsigned int recno, child;

   char mkey[1024];

   unsigned int mrecno, mchild;

   int er;

   if ((er = _ntx_free_page(ClipMachineMemory, ro, &lnode, __PROC__)))
      return er;

   _ntx_page(ro, rnode, &rpp);
   memcpy(pbuf, &rpp, sizeof(NTX_PAGE));
   pp = (NTX_PAGE *) pbuf;
   count = _rdd_ushort(pp->nkeys);
   mediane = count / 2;

   _ntx_page(ro, lnode, &lpp);

   if (buckno == mediane)
   {
      memcpy(key, kk, ro->keysize);
      loc_write(rd->loc, (unsigned char *) key, ro->keysize);
      key[ro->keysize] = 0;
      recno = rn;
      rchild = cc;
   }
   else
   {
      bp = _ntx_buck(&rpp, mediane);
      memcpy(key, bp->key, ro->keysize);
      key[ro->keysize] = 0;
      recno = _rdd_uint(bp->recno);
      rchild = _rdd_uint(bp->left);
   }

   child = lnode;

   bp = _ntx_buck(pp, count);
   if ((er = _ntx_init_page(ClipMachineMemory, ro, &rpp, _rdd_uint(bp->left), __PROC__)))
      return er;
   if ((er = _ntx_init_page(ClipMachineMemory, ro, &lpp, rchild, __PROC__)))
      return er;

   for (i = 0, j = 0; i < mediane; i++, j++)
   {
      if (i == buckno)
	 if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &lpp, lnode, kk, rn, cc, j++, 0, 1, __PROC__)))
	    return er;
      bp = _ntx_buck(pp, i);
      mrecno = _rdd_uint(bp->recno);
      mchild = _rdd_uint(bp->left);
      memcpy(mkey, bp->key, ro->keysize);
      mkey[ro->keysize] = 0;
      if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &lpp, lnode, mkey, mrecno, mchild, j, 0, 0, __PROC__)))
	 return er;
   }
   j = 0;
   if (buckno == mediane)
   {
      bp = _ntx_buck(pp, mediane);
      mrecno = _rdd_uint(bp->recno);
      mchild = _rdd_uint(bp->left);
      memcpy(mkey, bp->key, ro->keysize);
      mkey[ro->keysize] = 0;
      if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &rpp, rnode, mkey, mrecno, mchild, j++, 0, 0, __PROC__)))
	 return er;
   }
   for (i = mediane + 1; i < count; i++, j++)
   {
      if (i == buckno)
	 if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &rpp, rnode, kk, rn, cc, j++, 0, 1, __PROC__)))
	    return er;
      bp = _ntx_buck(pp, i);
      mrecno = _rdd_uint(bp->recno);
      mchild = _rdd_uint(bp->left);
      memcpy(mkey, bp->key, ro->keysize);
      mkey[ro->keysize] = 0;
      if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &rpp, rnode, mkey, mrecno, mchild, j, 0, 0, __PROC__)))
	 return er;
   }
   if (buckno == count)
      if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &rpp, rnode, kk, rn, cc, j, 0, 1, __PROC__)))
	 return er;

   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, lnode, sizeof(NTX_PAGE), (char *) &lpp, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, rnode, sizeof(NTX_PAGE), (char *) &rpp, __PROC__)))
      return er;

   if (ro->level < 1)
   {
      unsigned int root;

      NTX_HEADER hdr;

      NTX_PAGE page;

      if ((er = _ntx_free_page(ClipMachineMemory, ro, &root, __PROC__)))
	 return er;

      if ((er = _ntx_init_page(ClipMachineMemory, ro, &page, rnode, __PROC__)))
	 return er;
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, root, sizeof(NTX_PAGE), (char *) &page, __PROC__)))
	 return er;
      _ntx_header(ro, &hdr);
      _rdd_put_uint(hdr.root, root);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
	 return er;

      ro->stack[0].page = root;
      ro->stack[0].pos = 0;

      loc_read(rd->loc, (unsigned char *) key, ro->keysize);
      if ((er = _ntx_add_node(ClipMachineMemory, rd, ro, key, recno, child, __PROC__)))
	 return er;
   }
   else
   {
      --(ro->level);
      loc_read(rd->loc, (unsigned char *) key, ro->keysize);
      if ((er = _ntx_add_node(ClipMachineMemory, rd, ro, key, recno, child, __PROC__)))
	 return er;
   }

   free(pbuf);
   return 0;
}
