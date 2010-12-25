static int
ntx_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_HEADER hdr;

   unsigned int recno, lastrec;

   int res, sok, out, er;

   _ntx_header(ro, &hdr);
/*	printf("go top\n? 'go top',recno(),bof()\n");*/
   ro->level = 0;
   if ((er = _ntx_first(ClipMachineMemory, ro, _rdd_uint(hdr.root), &recno, &out, __PROC__)))
      return er;
   if (out)
   {
      if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	 return er;
      rd->bof = rd->v_bof = rd->eof = 1;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
      return 0;
   }
   rd->bof = rd->v_bof = rd->eof = 0;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, recno, 1, __PROC__)))
      return er;
   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
      return er;
   if ((er = _ntx_checkscope(ClipMachineMemory, rd, ro, &sok, __PROC__)))
      return er;
   if (!res || !sok)
      if ((er = ntx_next(ClipMachineMemory, rd, ro, __PROC__)))
	 return er;
   return 0;
}
