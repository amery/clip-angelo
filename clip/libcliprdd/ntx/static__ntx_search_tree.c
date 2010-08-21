static int
_ntx_search_tree(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, char *key, int len, unsigned int recno, int *res, int exact, const char *__PROC__)
{
   NTX_PAGE page;

   NTX_BUCKET *buck;

   int no;

   long offs = ro->stack[ro->level].page;

   int ok, er;

   _ntx_page(ro, offs, &page);
   *res = _ntx_search_page(rd, ro, &page, key, len, recno, &no, exact);
   buck = _ntx_buck(&page, no);
   ro->stack[ro->level].pos = no;
   if (*res)
   {
      if (recno && !ro->unique)
      {
	 RDD_STACK stack[MAX_BTREE_DEEP];

	 int level = ro->level;

	 memcpy(stack, ro->stack, sizeof(stack));
	 if ((er = __ntx_tunenext(ClipMachineMemory, rd, ro, key, len, recno, &ok, __PROC__)))
	    return er;
	 if (!ok)
	 {
	    ro->level = level;
	    memcpy(ro->stack, stack, sizeof(stack));
	    if ((er = __ntx_tuneprev(ClipMachineMemory, rd, ro, key, len, recno, &ok, __PROC__)))
	       return er;
	 }
	 if (!ok)
	 {
	    ro->level = level;
	    memcpy(ro->stack, stack, sizeof(stack));
	    *res = 1;
	    return 0;
	 }
      }
      else if (!exact)
      {
	 if ((er = __ntx_tuneseek(ClipMachineMemory, rd, ro, key, len, __PROC__)))
	    return er;
      }
      *res = 0;
      return 0;
   }

   offs = _rdd_uint(buck->left);
   if (!offs)
   {
      if (no == _rdd_ushort(page.nkeys))
      {
	 ro->eof = 1;
      }
      *res = 1;
      return 0;
   }

   (ro->level)++;
   if (ro->level >= MAX_BTREE_DEEP)
   {
      *res = 1;
      return 0;
   }
   ro->stack[ro->level].page = offs;

   return _ntx_search_tree(ClipMachineMemory, rd, ro, key, len, recno, res, exact, __PROC__);
}
