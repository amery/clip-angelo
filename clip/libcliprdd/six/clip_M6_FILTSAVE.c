int
clip_M6_FILTSAVE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTSAVE";

   int h = _clip_parni(ClipMachineMemory, 1);

   char *fname = _clip_parc(ClipMachineMemory, 2);

   char *path = NULL;

   RDD_FILTER *fp;

   struct flock fl;

   char buf[4];

   int fd, er, bytes;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".flt", &path, NULL, EG_CREATE, __PROC__)))
      goto err;

#ifdef _WIN32
   fd = open(path, O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   fd = open(path, O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
   if (fd == -1)
   {
#ifdef _WIN32
      fd = open(path, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
      fd = open(path, O_CREAT | O_TRUNC | O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
   }
   if (fd == -1)
      goto err_create;

   fl.l_type = F_WRLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0;
   fl.l_len = 1;

   if (fcntl(fd, F_SETLKW, &fl))
   {
      er = rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_writelock);
      goto err;
   }
   if (ftruncate(fd, 0))
      goto err_create;
   if (write(fd, "CFLT", 4) == -1)
      goto err_write;
   if (write(fd, &fp->custom, 1) == -1)
      goto err_write;
   _rdd_put_ushort((unsigned char *) buf, strlen(fp->sfilter));
   if (write(fd, buf, sizeof(short)) == -1)
      goto err_write;
   if (write(fd, fp->sfilter, _rdd_ushort((unsigned char *) buf) + 1) == -1)
      goto err_write;
   _rdd_put_uint((unsigned char *) buf, fp->size);
   if (write(fd, buf, sizeof(int)) == -1)
      goto err_write;
   if (fp->rmap)
   {
      bytes = (((fp->size + 1) >> 5) + 1) << 2;
      if (write(fd, fp->rmap, bytes) == -1)
	 goto err_write;
   }
   if (close(fd) == -1)
      goto err_write;
   _clip_retl(ClipMachineMemory, 1);
   free(path);
   return 0;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, er_create);
   goto err;
 err_write:
   er = rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_write);
 err:
   if (path)
      free(path);
   return er;
}
