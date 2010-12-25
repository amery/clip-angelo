static int
fpt_pack(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, int tmpfd, int bsize, ClipVar * block, int step, const char *__PROC__)
{
   FPT_HEADER hdr;

   unsigned int oldrecno = rd->recno;

   unsigned int lastrec;

   void *buf = NULL;

   int i;

   unsigned int memoid;

   int oldbsize = rm->blocksize;

   unsigned char h[8];

   unsigned int mlen;

   char s = 0xaf;

   unsigned short int vlen;

   int er;

   memset(&hdr, 0, sizeof(FPT_HEADER));
   if (bsize)
      rm->blocksize = bsize;
   if (step == 0)
      step = 1;
   if (block && block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      block = NULL;
   _rdd_put_backushort((unsigned char *) hdr.blocksize, rm->blocksize);
   if (rm->format == FLEX_MEMO || rm->format == CLIP_MEMO)
   {
      strcpy(hdr.sig, "FlexFile3");
      hdr.sig[9] = 3;
   }
   if (rm->format == CLIP_MEMO)
      strcpy(hdr.sig0, "Made by CLIP");
   if (rm->format == SIX_MEMO)
      strcpy(hdr.sig0, "SIxMemo");

   if (write(rm->file.fd, &hdr, sizeof(FPT_HEADER)) != sizeof(FPT_HEADER))
      goto err;
   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
      goto err1;

   for (rd->recno = 1; rd->recno <= lastrec; rd->recno++)
   {
      for (i = 0; i < rd->nfields; i++)
      {
	 if (strchr("MPG", rd->fields[i].type))
	 {
	    if ((er = rd->vtbl->getmemoid(ClipMachineMemory, rd, i, &memoid, NULL, __PROC__)))
	       goto err1;
	    if (lseek(tmpfd, memoid * oldbsize, SEEK_SET) != memoid * oldbsize)
	       goto err;
	    if (read(tmpfd, h, 8) != 8)
	       goto err;
	    mlen = _rdd_backuint(h + 4);
	    if (!(buf = malloc(mlen)))
	       goto err;
	    if (read(tmpfd, buf, mlen) != mlen)
	       goto err;

	    memoid = lseek(rm->file.fd, 0, SEEK_CUR) / rm->blocksize;
	    if (errno)
	       goto err;
	    if ((er = rd->vtbl->setmemoid(ClipMachineMemory, rd, i, memoid, __PROC__)))
	       goto err1;
	    if (write(rm->file.fd, h, 8) != 8)
	       goto err;
	    if (write(rm->file.fd, buf, mlen) != mlen)
	       goto err;
	    mlen = rm->blocksize - ((mlen + 9) % rm->blocksize);
	    lseek(rm->file.fd, mlen, SEEK_CUR);
	    if (errno)
	       goto err;
	    if (write(rm->file.fd, &s, 1) != 1)
	       goto err;
	    free(buf);
	    buf = NULL;
	 }
	 else if ((rd->fields[i].type == 'V') && (rd->fields[i].len >= 6))
	 {
	    if ((er = rd->vtbl->getmemoid(ClipMachineMemory, rd, i, &memoid, &vlen, __PROC__)))
	       goto err1;
	    if (vlen <= rd->fields[i].len - 2)
	       continue;
	    vlen -= rd->fields[i].len - 6;
	    if (lseek(tmpfd, memoid * oldbsize, SEEK_SET) != memoid * oldbsize)
	       goto err;

	    if (!(buf = malloc(vlen)))
	       goto err;
	    if (read(tmpfd, buf, vlen) != vlen)
	       goto err;

	    memoid = lseek(rm->file.fd, 0, SEEK_CUR) / rm->blocksize;
	    if (errno)
	       goto err;
	    if ((er = rd->vtbl->setmemoid(ClipMachineMemory, rd, i, memoid, __PROC__)))
	       goto err1;
	    if (write(rm->file.fd, buf, vlen) != vlen)
	       goto err;
	    if (vlen % rm->blocksize)
	    {
	       mlen = rm->blocksize - (vlen % rm->blocksize);
	       lseek(rm->file.fd, mlen - 1, SEEK_CUR);
	       if (errno)
		  goto err;
	       if (write(rm->file.fd, "", 1) != 1)
		  goto err;
	    }
	    free(buf);
	    buf = NULL;
	 }
      }
      if (block && rd->recno % step == 0)
      {
	 ClipVar vv;

	 if ((er = rdd_calc(ClipMachineMemory, rd->area, block, &vv, 0)))
	    goto err1;
	 _clip_destroy(ClipMachineMemory, &vv);
      }
   }
   memoid = lseek(rm->file.fd, 0, SEEK_CUR) / rm->blocksize;
   if (lseek(rm->file.fd, 0, SEEK_SET) != 0)
      goto err;
   _rdd_put_backuint(h, memoid);
   if (write(rm->file.fd, h, 4) != 4)
      goto err;
   rd->recno = oldrecno;
   return 0;
 err:
   er = rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, "I/O error");
 err1:
   if (buf)
      free(buf);
   rd->recno = oldrecno;
   return er;
}
