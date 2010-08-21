static int
_hs_getfuu(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int *poffs, const char *__PROC__)
{
   unsigned char fuu[4];

   int er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 2, 4, fuu, __PROC__)))
      return er;
   *poffs = _rdd_uint(fuu);
   if (*poffs == 0)
   {
      int delta;

      unsigned int fsize;

      if (hs->file.md == (caddr_t) - 1)
      {
	 struct stat st;

	 fstat(hs->file.fd, &st);
	 fsize = st.st_size;
      }
      else
	 fsize = hs->file.mapsize;

      delta = (fsize / hs->pagesize) / 5;
      if (delta < 1)
	 delta = 1;
      *poffs = fsize;
      if ((er = rdd_write(ClipMachineMemory, &hs->file, *poffs + delta * hs->pagesize, 0, "", __PROC__)))
	 return er;
      if (delta > 1)
      {
	 int i;

	 _rdd_put_uint(fuu, *poffs + hs->pagesize);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, 2, 4, fuu, __PROC__)))
	    return er;
	 for (i = 1; i < delta; i++)
	 {
	    if (i < delta - 1)
	       _rdd_put_uint(fuu, *poffs + (i + 1) * hs->pagesize);
	    else
	       _rdd_put_uint(fuu, 0);
	    if ((er = rdd_write(ClipMachineMemory, &hs->file, *poffs + i * hs->pagesize, 4, fuu, __PROC__)))
	       return er;
	 }
      }
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &hs->file, *poffs, 4, fuu, __PROC__)))
	 return er;
      if ((er = rdd_write(ClipMachineMemory, &hs->file, 2, 4, fuu, __PROC__)))
	 return er;
   }
   return 0;
}
