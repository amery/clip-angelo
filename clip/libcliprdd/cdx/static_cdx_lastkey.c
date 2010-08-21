static int
cdx_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__)
{
   unsigned int b = 1, e = 0;

   int found, er;

   CDX_HEADER hdr;

   _CDX_HEADER(ro, &hdr);
   if (ro->scopetop)
   {
      ro->stack[0].page = _rdd_uint(hdr.root);
      ro->level = 0;
      if (!ro->descend)
      {
	 if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	    return er;
      }
      else
      {
	 if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	    return er;
      }
      if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &b, __PROC__)))
	 return er;
   }

   if (ro->scopebottom)
   {
      ro->stack[0].page = _rdd_uint(hdr.root);
      ro->level = 0;
      if (!ro->descend)
      {
	 if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopebottom, ro->sbotlen, 0, &found, NULL, __PROC__)))
	    return er;
      }
      else
      {
	 if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopebottom, ro->sbotlen, 0, &found, NULL, __PROC__)))
	    return er;
      }
      if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &e, __PROC__)))
	 return er;
   }
   else
   {
      int out;

      if (!ro->descend)
      {
	 if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
	    return er;
      }
      else
      {
	 if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	    return er;
      }
      if (!out)
	 if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &e, __PROC__)))
	    return er;
   }
   (*lastkey) = max(0, (int) (e - b + 1));
   return 0;
}
