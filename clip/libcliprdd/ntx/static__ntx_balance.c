static int
_ntx_balance(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_PAGE pp, upp, rpp, tpp;

   int count, uno, ucount, rno, rcount, tno;

   unsigned int page, upage, rpage, right, tpage;

   NTX_BUCKET *bp;

   NTX_BUCKET *ubp;

   NTX_BUCKET *rbp;

   NTX_BUCKET *tbp;

   int er;

   page = ro->stack[ro->level].page;
   _ntx_page(ro, page, &pp);
   count = _rdd_ushort(pp.nkeys);

   if (ro->level < 1)
      return 0;
   if (count >= ro->halfpage)
      return 0;

   upage = ro->stack[ro->level - 1].page;
   uno = ro->stack[ro->level - 1].pos;
   _ntx_page(ro, upage, &upp);
   ucount = _rdd_ushort(upp.nkeys);
   ubp = _ntx_buck(&upp, ucount);
   right = _rdd_uint(ubp->left);

   ubp = _ntx_buck(&upp, uno);

   if (ucount < 1 && !right)
   {
      int i;

      _rdd_put_uint(ubp->left, 0);
      for (i = 0; i < count; ++i)
      {
	 char key[1024];

	 unsigned int recno;

	 bp = _ntx_buck(&pp, i);
	 memcpy(key, bp->key, ro->keysize);
	 key[ro->keysize] = 0;
	 recno = _rdd_uint(bp->recno);

	 if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &upp, upage, key, recno, 0, i, 0, 0, __PROC__)))
	    return er;
      }
      if ((er = _ntx_put_free_page(ClipMachineMemory, ro, page, __PROC__)))
	 return er;
      return 0;
   }

   if (uno > 0)
      rno = uno - 1;
   else if (uno < ucount - 1)
      rno = uno + 1;
   else if ((uno < ucount) && right)
      rno = ucount;
   else
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);

   rbp = _ntx_buck(&upp, rno);
   rpage = _rdd_uint(rbp->left);

   _ntx_page(ro, rpage, &rpp);
   rcount = _rdd_ushort(rpp.nkeys);

   if (uno > rno)
   {
      tpage = rpage;
      rpage = page;
      page = tpage;

      memcpy((void *) &tpp, (void *) &rpp, sizeof(NTX_PAGE));
      memcpy((void *) &rpp, (void *) &pp, sizeof(NTX_PAGE));
      memcpy((void *) &pp, (void *) &tpp, sizeof(NTX_PAGE));

      tno = rno;
      rno = uno;
      uno = tno;

      tno = rcount;
      rcount = count;
      count = tno;

      tbp = rbp;
      rbp = ubp;
      ubp = tbp;
   }

   bp = _ntx_buck(&pp, count);
   if (count + rcount < ro->fullpage)
   {
      /* join pages */
      int i, j, sum;

      char key[1024];

      unsigned int recno;

      unsigned int child;

      sum = count + rcount + 1;

      _rdd_put_uint(rbp->left, page);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, upage, sizeof(NTX_PAGE), (char *) &upp, __PROC__)))
	 return er;

      memcpy(key, ubp->key, ro->keysize);
      key[ro->keysize] = 0;
      recno = _rdd_uint(ubp->recno);
      child = _rdd_uint(bp->left);

      if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &pp, page, key, recno, child, count, 0, 0, __PROC__)))
	 return er;

      for (i = count + 1, j = 0; i <= sum; i++, j++)
      {
	 bp = _ntx_buck(&rpp, j);
	 memcpy(key, bp->key, ro->keysize);
	 key[ro->keysize] = 0;
	 recno = _rdd_uint(bp->recno);
	 child = _rdd_uint(bp->left);

	 if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &pp, page, key, recno, child, i, i == sum, 0, __PROC__)))
	    return er;
      }

      if ((er = _ntx_remove_node(ClipMachineMemory, ro, upage, &upp, uno, __PROC__)))
	 return er;
      if ((er = _ntx_put_free_page(ClipMachineMemory, ro, rpage, __PROC__)))
	 return er;
      if ((ro->level == 1) && (ucount == 1))
      {
	 /* New root */
	 NTX_HEADER hdr;

	 _ntx_header(ro, &hdr);
	 _rdd_put_uint(hdr.root, page);
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
	    return er;
      }
   }
   else
   {
      /* balance pages */
      int i, j, sum, mediane;

      char key[1024];

      unsigned int recno;

      unsigned int child;

      sum = count + rcount;
      mediane = sum / 2;

      if (count > mediane)
      {
	 /* pp -> rpp */
	 bp = _ntx_buck(&pp, count);
	 memcpy(key, ubp->key, ro->keysize);
	 key[ro->keysize] = 0;
	 recno = _rdd_uint(ubp->recno);
	 child = _rdd_uint(bp->left);
	 if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &rpp, rpage, key, recno, child, 0, 0, 0, __PROC__)))
	    return er;

	 for (i = count - 1; i > mediane; i--)
	 {
	    bp = _ntx_buck(&pp, i);
	    memcpy(key, bp->key, ro->keysize);
	    key[ro->keysize] = 0;
	    recno = _rdd_uint(bp->recno);
	    child = _rdd_uint(bp->left);
	    if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &rpp, rpage, key, recno, child, 0, 0, 0, __PROC__)))
	       return er;
	 }

	 bp = _ntx_buck(&pp, mediane);
	 memcpy(ubp->key, bp->key, ro->keysize);
	 _rdd_put_uint(ubp->recno, _rdd_uint(bp->recno));
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, upage, sizeof(NTX_PAGE), (char *) &upp, __PROC__)))
	    return er;

	 for (i = count; i > mediane; i--)
	    if ((er = _ntx_remove_node(ClipMachineMemory, ro, page, &pp, i, __PROC__)))
	       return er;
	 _rdd_put_uint(bp->recno, 0);
	 memset(bp->key, 0, ro->keysize);
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(NTX_PAGE), (char *) &pp, __PROC__)))
	    return er;
      }
      else if (count < mediane)
      {
	 /* rpp -> pp */
	 bp = _ntx_buck(&pp, count);
	 memcpy(key, ubp->key, ro->keysize);
	 key[ro->keysize] = 0;
	 recno = _rdd_uint(ubp->recno);
	 child = _rdd_uint(bp->left);
	 if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &pp, page, key, recno, child, count, 0, 0, __PROC__)))
	    return er;

	 for (i = count + 1, j = 0; i < mediane; i++, j++)
	 {
	    bp = _ntx_buck(&rpp, j);
	    memcpy(key, bp->key, ro->keysize);
	    key[ro->keysize] = 0;
	    recno = _rdd_uint(bp->recno);
	    child = _rdd_uint(bp->left);

	    if ((er = _ntx_put_node(ClipMachineMemory, rd, ro, &pp, page, key, recno, child, i, 0, 0, __PROC__)))
	       return er;
	 }

	 bp = _ntx_buck(&rpp, mediane - (count + 1));
	 memcpy(ubp->key, bp->key, ro->keysize);
	 _rdd_put_uint(ubp->recno, _rdd_uint(bp->recno));
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, upage, sizeof(NTX_PAGE), (char *) &upp, __PROC__)))
	    return er;

	 bp = _ntx_buck(&rpp, j);
	 right = _rdd_uint(bp->left);
	 bp = _ntx_buck(&pp, i);
	 _rdd_put_uint(bp->left, right);
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(NTX_PAGE), (char *) &pp, __PROC__)))
	    return er;

	 for (i = count + 1; i <= mediane; i++)
	    if ((er = _ntx_remove_node(ClipMachineMemory, ro, rpage, &rpp, 0, __PROC__)))
	       return er;
      }
   }

   --(ro->level);

   return _ntx_balance(ClipMachineMemory, rd, ro, __PROC__);
}
