static int
ntx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int res = 0, sok, er;

   unsigned int lastrec;

   rd->bof = rd->v_bof = 0;
   if (rd->eof)
      return 0;
   rd->bof = rd->v_bof = rd->eof = 0;
/*	printf("skip 1\n? 'skip 1',recno(),bof()\n");*/
   if (rd->shared || !ro->valid_stack)
   {
      char cnt[2];

      int r;

      er = rdd_read(ClipMachineMemory, &ro->index->file, 2, 2, cnt, __PROC__);
      if (!ro->valid_stack || ro->cnt != _rdd_ushort((unsigned char *) cnt))
      {
	 NTX_HEADER hdr;

	 _ntx_header(ro, &hdr);
	 ro->stack[0].page = _rdd_uint(hdr.root);
	 ro->level = 0;
	 ro->curoffs = 0;

	 if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
	    return er;

	 if ((er = _ntx_search_tree(ClipMachineMemory, rd, ro, ro->key, ro->keysize, rd->recno, &r, 0, __PROC__)))
	    return er;
	 if (r)
	 {
	    if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	       return er;
	    if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	       return er;
	    rd->eof = 1;
	    ro->valid_stack = 0;
	    return 0;
	 }
	 ro->cnt = _rdd_ushort((unsigned char *) cnt);
      }
   }

   while (1)
   {
      int out;

      if ((er = __ntx_next(ClipMachineMemory, rd, ro, &out, __PROC__)))
	 return er;
      if (out)
      {
	 if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	    return er;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 rd->eof = 1;
	 ro->valid_stack = 0;
	 break;
      }
      if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
	 return er;
      if ((er = _ntx_checkscope(ClipMachineMemory, rd, ro, &sok, __PROC__)))
	 return er;
      if (res && sok)
      {
	 ro->valid_stack = 1;
	 break;
      }
   }
   return 0;
}
