int
rdd_setmemo(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *driver, const char *name, const char *__PROC__)
{
   RDD_MEMO *rm = (RDD_MEMO *) calloc(1, sizeof(RDD_MEMO));

   struct stat st;

   int er = EG_UNSUPPORTED;

   if (rd->memo)
   {
      er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, "Memo file is already opened");
      goto err;
   }

   memset(rm, 0, sizeof(RDD_MEMO));
   rm->loc = rd->loc;
   if (!(rm->vtbl = rdd_memodriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, rm->vtbl->suff, &rm->path, &rm->name, EG_OPEN, __PROC__)))
      goto err;

   rm->file.filehash = _clip_hashstr(rm->path);

   if ((er = rdd_open(ClipMachineMemory, rm->path, rd->readonly, rd->shared, &rm->file.fd, __PROC__)))
      goto err;

   if (fstat(rm->file.fd, &st) == -1)
      goto err_open;
   rm->file.mapsize = st.st_size;

   rm->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
      rm->file.md = (caddr_t) mmap(0, rm->file.mapsize, PROT_READ | (rd->readonly ? 0 : PROT_WRITE), MAP_SHARED, rm->file.fd, 0);
   /* If failed use non-mapped file (md==-1) */
#endif
   rd->memo = rm;

   if ((er = rm->vtbl->open(ClipMachineMemory, rd, rm, __PROC__)))
      goto err;

   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, rm->path);
 err:
   if (rm && rm->name)
      free(rm->name);
   if (rm && rm->path)
      free(rm->path);
#ifdef HAVE_MMAN_H
   if (rm && rm->file.md != (char *) -1)
      munmap(rm->file.md, rm->file.mapsize);
#endif
   if (rm && rm->file.fd != -1)
      _clip_close(ClipMachineMemory, rm->file.filehash, rm->file.fd);
   if (rm)
      free(rm);
   rd->memo = NULL;
   return er;
}
