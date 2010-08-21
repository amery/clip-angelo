static int
ntx_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__)
{
   NTX_HEADER hdr;

   int er;

   _ntx_header(ro, &hdr);
   *lastkey = 0;
   return _ntx_leftkeys(ClipMachineMemory, ro, _rdd_uint(hdr.root), lastkey, __PROC__);
}
