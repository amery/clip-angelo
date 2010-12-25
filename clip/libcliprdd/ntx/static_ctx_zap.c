static int
ctx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   CTX_HEADER chdr;

   NTX_HEADER *hdr;

   NTX_PAGE page;

   NTX_BUCKET *buck;

   int i, j, offs, er;

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CTX_HEADER), (char *) &chdr, __PROC__)))
      return er;

   hdr = (NTX_HEADER *) malloc(chdr.ntags * sizeof(NTX_HEADER));
   for (i = 0; i < chdr.ntags; i++)
   {
      if ((er = rdd_read(ClipMachineMemory, &ri->file, _rdd_uint(chdr.tags[i].header), sizeof(NTX_HEADER), &hdr[i], __PROC__)))
	 return er;
   }

   if ((er = rdd_trunc(ClipMachineMemory, &ri->file, sizeof(CTX_HEADER) + chdr.ntags * (sizeof(NTX_HEADER) + sizeof(NTX_PAGE)), __PROC__)))
      return er;

   for (i = 0; i < chdr.ntags; i++)
   {
      _rdd_put_uint(chdr.tags[i].header, sizeof(CTX_HEADER) + i * sizeof(NTX_HEADER));
      ri->orders[i]->header = _rdd_uint(chdr.tags[i].header);
      _rdd_put_uint(hdr[i].fuu, 0);
      _rdd_put_uint(hdr[i].root, sizeof(CTX_HEADER) + chdr.ntags * sizeof(NTX_HEADER) + i * sizeof(NTX_PAGE));
      if ((er = rdd_write(ClipMachineMemory, &ri->file, sizeof(CTX_HEADER) + i * sizeof(NTX_HEADER), sizeof(NTX_HEADER), &hdr[i], __PROC__)))
	 return er;

      memset(&page, 0, sizeof(NTX_PAGE));
      offs = 2 * (_rdd_ushort(hdr[i].fullpage) + 2);
      _rdd_put_ushort(page.nkeys, 0);

      buck = (NTX_BUCKET *) (((char *) &page) + offs);
      _rdd_put_uint(buck->left, 0);
      _rdd_put_uint(buck->recno, 0);

      for (j = 0; j <= _rdd_ushort(hdr[i].fullpage); ++j, offs += _rdd_ushort(hdr[i].itemsize))
	 _rdd_put_ushort(page.offs + j * 2, offs);

      if ((er = rdd_write(ClipMachineMemory, &ri->file, sizeof(CTX_HEADER) + chdr.ntags * sizeof(NTX_HEADER) + i * sizeof(NTX_PAGE), sizeof(NTX_PAGE), &page, __PROC__)))
	 return er;

      for (j = 0; j < ri->norders; j++)
      {
	 if (strcasecmp((const char *) chdr.tags[i].tagname, ri->orders[j]->name) == 0)
	 {
	    ri->orders[j]->header = sizeof(CTX_HEADER) + i * sizeof(NTX_HEADER);
	    break;
	 }
      }
   }
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
      return er;

   rd->bof = rd->v_bof = rd->eof = 1;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, 1, 0, __PROC__)))
      return er;
   return 0;
}
