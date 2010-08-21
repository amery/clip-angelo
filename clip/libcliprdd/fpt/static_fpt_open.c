static int
fpt_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, const char *__PROC__)
{
   FPT_HEADER hdr;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, sizeof(FPT_HEADER), &hdr, __PROC__)))
      return er;
   rm->blocksize = _rdd_backushort((unsigned char *) hdr.blocksize);
   if (memcmp(hdr.sig0, "SIxMemo", 7) == 0)
      rm->format = SIX_MEMO;
   else if (memcmp(hdr.sig0, "Made by CLIP", 12) == 0)
      rm->format = CLIP_MEMO;
   else if (memcmp(hdr.sig, "FlexFile3\03", 9) == 0)
      rm->format = FLEX_MEMO;

   /* Upgrade old dumb CLIP's FlexFile implementation to the right one. */
   /* Thanks to Przemek (Przemyslaw Czerpak <druzus@priv.onet.pl>) */
   if (rm->format == CLIP_MEMO && !rd->readonly && !hdr.sig0[12])
   {
      hdr.sig0[12] = '1';
      _rdd_put_uint((unsigned char *) hdr.flexsize, 0);
      _rdd_put_uint((unsigned char *) hdr.flexoffs, 0);
      if ((er = rdd_write(ClipMachineMemory, &rm->file, 0, sizeof(FPT_HEADER), &hdr, __PROC__)))
	 return er;
   }
   return 0;
}
