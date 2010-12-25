static int
__ntx_tuneseek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, char *key, int len, const char *__PROC__)
{
   NTX_PAGE page;

   NTX_BUCKET *buck;

   int c;

   int out, er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
   while (!c)
   {
      if ((er = __ntx_prev(ClipMachineMemory, rd, ro, &out, __PROC__)))
	 return er;
      if (out)
	 break;
      _ntx_page(ro, ro->stack[ro->level].page, &page);
      buck = _ntx_buck(&page, ro->stack[ro->level].pos);
      c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
   }
   if (c)
   {
      if ((er = __ntx_next(ClipMachineMemory, rd, ro, &out, __PROC__)))
	 return er;
   }
   return 0;
}
