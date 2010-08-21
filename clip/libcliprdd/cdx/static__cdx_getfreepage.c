static int
_cdx_getfreepage(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, unsigned int *page, const char *__PROC__)
{
   unsigned int fsize;

   char fuu[4];

   int er;

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 4, 4, fuu, __PROC__)))
      return er;
   *page = _rdd_uint((unsigned char *) fuu);
   if (!*page)
   {
      int delta;

      if (ri->file.md == (caddr_t) - 1)
      {
	 struct stat st;

	 fstat(ri->file.fd, &st);
	 fsize = st.st_size;
      }
      else
	 fsize = ri->file.mapsize;

      delta = (fsize / sizeof(CDX_PAGE)) / 5;
      if (delta < 1)
	 delta = 1;
      *page = fsize;
      if ((er = rdd_write(ClipMachineMemory, &ri->file, *page + delta * sizeof(CDX_PAGE), 0, "", __PROC__)))
	 return er;
      if (delta > 1)
      {
	 int i;

	 _rdd_put_uint((unsigned char *) fuu, *page + sizeof(CDX_PAGE));
	 if ((er = rdd_write(ClipMachineMemory, &ri->file, 4, 4, fuu, __PROC__)))
	    return er;
	 for (i = 1; i < delta; i++)
	 {
	    if (i < delta - 1)
	       _rdd_put_uint((unsigned char *) fuu, *page + (i + 1) * sizeof(CDX_PAGE));
	    else
	       _rdd_put_uint((unsigned char *) fuu, 0);
	    if ((er = rdd_write(ClipMachineMemory, &ri->file, *page + i * sizeof(CDX_PAGE), 4, fuu, __PROC__)))
	       return er;
	 }
      }
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &ri->file, *page, 4, fuu, __PROC__)))
	 return er;
      if ((er = rdd_write(ClipMachineMemory, &ri->file, 4, 4, fuu, __PROC__)))
	 return er;
   }
   return 0;
}
