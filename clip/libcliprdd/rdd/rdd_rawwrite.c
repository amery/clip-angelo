int
rdd_rawwrite(ClipMachine * ClipMachineMemory, RDD_DATA * rd, void *buf, int append, const char *__PROC__)
{
   int er;

   if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, buf, __PROC__)))
      return er;
   if (append)
   {
      unsigned char lr[4];

      long flen;

      _rdd_put_uint(lr, rd->recno);
      if ((er = rdd_write(ClipMachineMemory, &rd->file, 4, 4, lr, __PROC__)))
	 return er;
      rd->recno++;
      flen = rd->hdrsize + rd->recsize * rd->recno + 1;
      if (rd->file.md != (caddr_t) (-1))
      {
	 if (flen > rd->file.mapsize)
	 {
	    int delta = rd->recno / 5;

	    if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno + delta) - 1, 1, "", __PROC__)))
	       return er;
	 }
      }
   }
   rd->updated = 1;
   return 0;
}
