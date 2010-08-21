static int
_ntx_free_page(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int *page, const char *__PROC__)
{
   NTX_HEADER hdr;

   unsigned int fsize;

   int er;

   _ntx_header(ro, &hdr);
   *page = _rdd_uint(hdr.fuu);
   if (!*page)
   {
      int delta;

      if (ro->index->file.md == (caddr_t) - 1)
      {
	 struct stat st;

	 fstat(ro->index->file.fd, &st);
	 fsize = st.st_size;
      }
      else
	 fsize = ro->index->file.mapsize;

      delta = ((fsize - sizeof(NTX_HEADER)) / sizeof(NTX_PAGE)) / 5;
      if (delta < 1)
	 delta = 1;
      *page = fsize;
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, *page + delta * sizeof(NTX_PAGE), 0, "", __PROC__)))
	 return er;
      if (delta > 1)
      {
	 int i;

	 NTX_FREEPAGE fp;

	 memset(&fp, 0, sizeof(NTX_FREEPAGE));

	 _rdd_put_uint(hdr.fuu, *page + sizeof(NTX_PAGE));
	 for (i = 1; i < delta; i++)
	 {
	    if (i < delta - 1)
	       _rdd_put_backuint(fp.next, *page + (i + 1) * sizeof(NTX_PAGE));
	    else
	       _rdd_put_backuint(fp.next, 0);
	    if ((er = rdd_write(ClipMachineMemory, &ro->index->file, *page + i * sizeof(NTX_PAGE), sizeof(NTX_FREEPAGE), (char *) &fp, __PROC__)))
	       return er;
	 }
      }
   }
   else
   {
      char next[4];

      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, *page, 4, next, __PROC__)))
	 return er;
      _rdd_put_uint(hdr.fuu, _rdd_backuint((unsigned char *) next));
   }
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;
   return 0;
}
