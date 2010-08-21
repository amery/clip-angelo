int
rdd_ii_open(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__)
{
   RDD_INDEX *ri = NULL;

   struct stat st;

   int er = EG_UNSUPPORTED;

   ri = (RDD_INDEX *) calloc(1, sizeof(RDD_INDEX));
   memset(ri, 0, sizeof(RDD_INDEX));
   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_CREATE, __PROC__)))
      goto err;

   ri->file.filehash = _clip_hashstr(ri->path);

   if ((er = rdd_open(ClipMachineMemory, ri->path, 0, 0, &ri->file.fd, __PROC__)))
      goto err;

   if (fstat(ri->file.fd, &st) == -1)
      goto err_open;
   ri->file.mapsize = st.st_size;
   ri->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if (ClipMachineMemory->flags1 & MAP_FILE_FLAG)
      ri->file.md = (caddr_t) mmap(0, ri->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, ri->file.fd, 0);
#endif
   if ((er = ri->vtbl->ii_open(ClipMachineMemory, ri, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, ri, _C_ITEM_TYPE_I_INDEX, destroy_rdd_index));
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, ri->path);
 err:
   if (ri && ri->name)
      free(ri->name);
   if (ri && ri->path)
      free(ri->path);
#ifdef HAVE_MMAN_H
   if (ri && ri->file.md != (char *) -1)
      munmap(ri->file.md, ri->file.mapsize);
#endif
   if (ri && ri->file.fd != -1)
      _clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd);
   if (ri)
      free(ri);
   return er;
}
