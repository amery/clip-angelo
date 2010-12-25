int
clip_M6_FILTRESTORE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTRESTORE";

   char *fname = _clip_parc(ClipMachineMemory, 1);

   char *path = NULL;

   RDD_FILTER *fp = NULL;

   struct flock fl;

   char buf[4];

   int fd, er, bytes;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".flt", &path, NULL, EG_OPEN, __PROC__)))
      goto err;

#ifdef _WIN32
   fd = open(path, O_RDONLY | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   fd = open(path, O_RDONLY, ClipMachineMemory->fileCreateMode);
#endif

   if (fd == -1)
      goto err_open;

   fl.l_type = F_RDLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0;
   fl.l_len = 1;

   if (fcntl(fd, F_SETLKW, &fl))
   {
      er = rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_readlock);
      goto err;
   }

   fp = calloc(1, sizeof(RDD_FILTER));
   fp->fps = calloc(1, sizeof(RDD_FPS));
   if (read(fd, buf, 4) == -1)
      goto err_read;
   if (memcmp(buf, "CFLT", 4))
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, errno, __FILE__, __LINE__, __PROC__, er_badfltfile);
      goto err;
   }
   if (read(fd, &fp->custom, 1) == -1)
      goto err_read;
   if (read(fd, buf, sizeof(short)) == -1)
      goto err_read;
   fp->sfilter = (char *) calloc(1, _rdd_ushort((unsigned char *) buf) + 1);
   if (read(fd, fp->sfilter, _rdd_ushort((unsigned char *) buf) + 1) == -1)
      goto err_read;
   if (read(fd, buf, sizeof(int)) == -1)
      goto err_read;
   fp->size = _rdd_uint((unsigned char *) buf);
   if (fp->size)
   {
      bytes = (((fp->size + 1) >> 5) + 1) << 2;
      fp->rmap = calloc(1, bytes);
      if (read(fd, fp->rmap, bytes) == -1)
	 goto err_read;
   }
   fp->nfps = 1;
   fp->handle = _clip_store_c_item(ClipMachineMemory, fp, _C_ITEM_TYPE_RYO, destroy_ryo);
   if (close(fd) == -1)
      goto err_read;
   free(path);
   _clip_retni(ClipMachineMemory, fp->handle);
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, er_open);
   goto err;
 err_read:
   er = rdd_err(ClipMachineMemory, EG_READ, errno, __FILE__, __LINE__, __PROC__, er_read);
 err:
   if (path)
      free(path);
   if (fp)
   {
      if (fp->fps)
	 free(fp->fps);
      if (fp->sfilter)
	 free(fp->sfilter);
      if (fp->rmap)
	 free(fp->rmap);
      free(fp);
   }
   return er;
}
