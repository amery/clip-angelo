int
rdd_pack(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   char *s;

   char tmp[PATH_MAX];

   int tmpfd;

   struct stat st;

   int r, er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_event(ClipMachineMemory, EVENT_PACK, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if (rd->shared)
      return rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

#ifdef HAVE_MMAN_H
   if (rd->file.md != (caddr_t) - 1)
   {
      if (munmap(rd->file.md, rd->file.mapsize) == -1)
	 goto err;
   }
#endif
   if (_clip_close(ClipMachineMemory, rd->file.filehash, rd->file.fd) == -1)
      goto err;

   strcpy(tmp, rd->path);
   s = strrchr(tmp, '/');
   sprintf(s + 1, "data%lx", (long) getpid());
   sprintf(s + 9, ".%lx", (long) random());
   s[13] = 0;

   if (rename(rd->path, tmp) == -1)
      goto err;

#ifdef _WIN32
   rd->file.fd = _clip_creat(ClipMachineMemory, rd->path, O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode, !rd->shared);
#else
   rd->file.fd = _clip_creat(ClipMachineMemory, rd->path, O_RDWR, ClipMachineMemory->fileCreateMode, !rd->shared);
#endif
   if (rd->file.fd == -1)
      goto err_create;
   rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__);
   if ((er = rdd_open(ClipMachineMemory, tmp, 0, 0, &tmpfd, __PROC__)))
      return er;

   if ((er = rd->vtbl->pack(ClipMachineMemory, rd, tmpfd, __PROC__)))
      return er;
   if (_clip_close(ClipMachineMemory, _clip_hashstr(tmp), tmpfd) == -1)
      goto err;

   if (fstat(rd->file.fd, &st) == -1)
      goto err;
   rd->file.mapsize = st.st_size;
   rd->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if (ClipMachineMemory->flags1 & MAP_FILE_FLAG)
      rd->file.md = (caddr_t) mmap(0, rd->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, rd->file.fd, 0);
#endif
   if (remove(tmp) == -1)
      goto err;
   rd->eof = 1;
   if ((er = rdd_reindex(ClipMachineMemory, rd, __PROC__)))
      return er;
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
 err_create:
   return rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, rd->path);
}
