static int
_ntx_init_page(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, NTX_PAGE * page, unsigned int rchield, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_BUCKET *buck;

   unsigned short int offs;

   int i, er;

   _ntx_header(ro, &hdr);
   offs = 2 * (ro->fullpage + 2);
   memset(page, 0, sizeof(NTX_PAGE));
   _rdd_put_ushort(page->nkeys, 0);

   buck = (NTX_BUCKET *) (((char *) page) + offs);
   _rdd_put_uint(buck->left, rchield);
   _rdd_put_uint(buck->recno, 0);

   for (i = 0; i <= ro->fullpage; ++i, offs += _rdd_ushort(hdr.itemsize))
      _rdd_put_ushort(page->offs + i * 2, offs);

   return 0;
}
