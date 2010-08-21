static int
fpt_zap(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, const char *__PROC__)
{
   FPT_HEADER hdr;

   int fuu, er;

   fuu = sizeof(FPT_HEADER) / rm->blocksize;
   fuu += (fuu * rm->blocksize < sizeof(FPT_HEADER)) ? 1 : 0;
   if ((er = rdd_trunc(ClipMachineMemory, &rm->file, fuu * rm->blocksize, __PROC__)))
      return er;
   if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, sizeof(FPT_HEADER), &hdr, __PROC__)))
      return er;
   _rdd_put_backuint((unsigned char *) hdr.fuu, fuu);
   _rdd_put_uint((unsigned char *) hdr.flexsize, 0);
   _rdd_put_uint((unsigned char *) hdr.flexoffs, 0);
   return rdd_write(ClipMachineMemory, &rm->file, 0, sizeof(FPT_HEADER), &hdr, __PROC__);
}
