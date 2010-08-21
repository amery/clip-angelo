static int
dbf_pack(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int tfd, const char *__PROC__)
{
   DBF_HEADER hdr;

   unsigned char *buf = NULL;

   unsigned int oldnrecs;

   unsigned int newnrecs = 0;

   int rs, i;

   if (read(tfd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   oldnrecs = _rdd_uint(hdr.recs);
   _rdd_put_uint(hdr.recs, 0);
   if (write(rd->file.fd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   buf = malloc(_rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER));
   if (read(tfd, buf, _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER)) != _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER))
      goto err;
   if (write(rd->file.fd, buf, _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER)) != _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER))
      goto err;
   free(buf);
   rs = _rdd_ushort(hdr.recsize);
   buf = malloc(rs);
   for (i = 0; i < oldnrecs; i++)
   {
      if (read(tfd, buf, rs) != rs)
	 goto err;
      if (*buf == 0x20)
      {
	 if (write(rd->file.fd, buf, rs) != rs)
	    goto err;
	 newnrecs++;
      }
   }
   free(buf);
   buf = NULL;
   if (lseek(rd->file.fd, 0, SEEK_SET) == -1)
      goto err;
   if (read(rd->file.fd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   _rdd_put_uint(hdr.recs, newnrecs);
   if (lseek(rd->file.fd, 0, SEEK_SET) == -1)
      goto err;
   if (write(rd->file.fd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   return 0;
 err:
   if (buf)
      free(buf);
   return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}
